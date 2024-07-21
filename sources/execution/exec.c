/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdanish <mdanish@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 14:47:16 by maabdull          #+#    #+#             */
/*   Updated: 2024/07/19 17:59:13 by mdanish          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"

/**
 * This file contains the functions to execute the various command types
 */

// Mallocs the full path to a command
// or returns NULL if no command was found
char	*find_cmd(char *cmd)
{
	char	*final_cmd;
	char	**dirs;
	char	*path;
	int		j;

	final_cmd = NULL;
	path = getenv("PATH");
	if (!path)
		return (NULL);
	dirs = ft_split(path, ':');
	j = 0;
	while (dirs[j])
	{
		final_cmd = ft_char_strjoin(dirs[j], cmd, '/');
		if (access(final_cmd, X_OK) == 0)
			break ;
		free(final_cmd);
		final_cmd = NULL;
		j++;
	}
	ft_free_2d_arr(dirs);
	return (final_cmd);
}

char	**convert_cmd_exec(t_token *tokens)
{
	int	i;
	t_token *current;
	char	**str_tokens;

	i = 0;
	current = tokens;
	while (current)
	{
		i++;
		current = current->next;
	}
	str_tokens = ft_calloc(i + 1, sizeof(char *));
	// TODO: Handle malloc fail properly
	if (!str_tokens)
		exit(1);
	i = 0;
	current = tokens;
	while (current)
	{
		str_tokens[i] = ft_strdup(current->content);
		current = current->next;
		i++;
	}
	str_tokens[i] = NULL;
	return (str_tokens);
}

void	exec_pipe(t_cmd_expr *cmd, char **env)
{
	int	fd[2];
	int	pid1;
	int	pid2;

	if (pipe(fd) < 0)
		return (ft_putendl_fd("pipe creation", 2));
	receive_signal(CHILD);
	pid1 = fork();
	if (pid1 < 0)
		return (ft_putendl_fd("fork", 2));
	else if (pid1 == 0)
	{
		dup2(fd[1], STDOUT_FILENO);
		close(fd[0]);
		close(fd[1]);
		run_cmd(cmd->cmd_left, env);
		exit(0);
	}
	receive_signal(CHILD);
	pid2 = fork();
	if (pid2 < 0)
		return (ft_putendl_fd("fork", 2));
	else if (pid2 == 0)
	{
		dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
		close(fd[1]);
		run_cmd(cmd->cmd_right, env);
		exit(0);
	}
	close(fd[0]);
	close(fd[1]);
	waitpid(pid1, NULL, 0);
	waitpid(pid2, NULL, 0);
}

/**
 * @brief Check if the provided command is of a redirection type
 *
 * @param cmd
 * @return bool
 */
bool	is_redirection_cmd(t_cmd *cmd)
{
	if (cmd->type == CMD_LESS || \
		cmd->type == CMD_HEREDOC || \
		cmd->type == CMD_GREAT || \
		cmd->type == CMD_DBL_GREAT)
		return (true);
	return (false);
}

/**
 * @brief Open all the files in the redirection command tree.
 *
 * Recursively opens all the files in the provided command tree. 
 * This is necessary because every file must get created but only the first
 * one should contain the command output.
 * This is done by checking if the command contains a cmd variable that is of
 * a redirection type. (There is room for optimization here because it gets
 * opened twice if its not a redir cmd, once here and once in the exec_redir
 * function). If it is a redirection command, it calls itself with the cmd
 * variable in the command.
 * This goes on until a non-redirection command is found, which opens the file
 * associated with the command and returns the command which is to be used
 * with the first file in the command tree to mimic bash behaviour.
 * (The tree structure causes the file passed in at the end to be the first
 * file in the tree.)
 *
 * Checks if the current commands cmd variable is not of a redirection type
 * and opens the file specified in the command and returns the command.
 * If the command is a redirection command, calls the function again with the
 * cmd variable in this command and stores it's output in the final_cmd var.
 * Opens the file specified in the command and returns final_cmd.
 *
 * @param cmd 
 * @return t_cmd*
 */
t_cmd_redir	*open_files(t_cmd_redir *cmd)
{
	t_cmd_redir	*final_cmd;

	if (!is_redirection_cmd(cmd->cmd))
	{
		if (cmd->type == CMD_DBL_GREAT)
			open(cmd->file, O_WRONLY|O_CREAT|O_APPEND, 0666);
		else if (cmd->type == CMD_GREAT)
			open(cmd->file, O_WRONLY|O_CREAT|O_TRUNC, 0666);
		else if (cmd->type == CMD_LESS)
			open(cmd->file, O_RDONLY);
		return (cmd);
	}
	else
		final_cmd = open_files((t_cmd_redir *) cmd->cmd);
	if (cmd->type == CMD_DBL_GREAT)
		open(cmd->file, O_WRONLY|O_CREAT|O_APPEND, 0666);
	else if (cmd->type == CMD_GREAT)
		open(cmd->file, O_WRONLY|O_CREAT|O_TRUNC, 0666);
	else if (cmd->type == CMD_LESS)
		open(cmd->file, O_RDONLY);
	return (final_cmd);
}

/**
 * @brief Execute a redirection command.
 *
 * This function is run when a redirection command is encountered.
 * It opens the file specified in the command and uses that for redirection
 *
 * Creates an empty redirection command struct to save the first command in.
 * Opens all the files it finds in the redirection tree using the open_files function.
 * Checks and opens the first commands file with the appropriate parameters.
 * dup2's the opened file descriptor to the specified file descriptor of the final command.
 * Executes the command stored in the final commands command variable using the run_cmd function.
 *
 * @param cmd
 * @param env
 */
void	exec_redir(t_cmd_redir *cmd, char **env)
{
	int	fd_redirect;
	t_cmd_redir *cmd_head;

	fd_redirect = 0;
	cmd_head = cmd;
	cmd = open_files(cmd);
	if (cmd_head->type == CMD_DBL_GREAT)
		fd_redirect = open(cmd_head->file, O_WRONLY|O_CREAT|O_APPEND, 0666);
	else if (cmd_head->type == CMD_GREAT)
		fd_redirect = open(cmd_head->file, O_WRONLY|O_CREAT|O_TRUNC, 0666);
	else if (cmd_head->type == CMD_LESS)
		fd_redirect = open(cmd_head->file, O_RDONLY);
	dup2(fd_redirect, cmd->fd);
	run_cmd(cmd->cmd, env);
}

/**
 * @brief Get the length of the longer string out of the provided two strings
 * 
 * @param str1 
 * @param str2 
 * @return int 
 */
int	get_longer_length(char *str1, char *str2)
{
	int	len1;
	int	len2;

	len1 = ft_strlen(str1);
	len2 = ft_strlen(str2);
	if (len1 > len2)
		return (len1);
	return (len2);
}

/**
 * @brief Execute a heredoc command.
 * 
 * Infinitely displays a prompt that accepts user input until the delimiter is 
 * found.
 * This user input is stored in the write end of a pipe using ft_putendl_fd 
 * which is accumulated due to how pipes manage buffers, until it is read from
 * the read end.
 * Once the delimiter is found, the read end of the pipe is dup2'd into the 
 * stdin which makes it the input for the command to be executed.
 * 
 * @param cmd 
 * @param env 
 */
void	exec_heredoc(t_cmd_heredoc *cmd, char **env)
{
	char	*line;
	int		fd[2];

	if (pipe(fd) < 0)
		perror("pipe creation");
	ft_putendl_fd("Running heredoc", 1);
	while (true)
	{
		line = readline("> ");
		if (ft_strncmp(line, cmd->delimiter, !get_longer_length(line, cmd->delimiter)))
			break ;
		ft_putendl_fd(line, fd[1]);
		free(line);
	}
	free(line);
	dup2(fd[0], STDIN_FILENO);
	close(fd[0]);
	close(fd[1]);
	run_cmd(cmd->cmd, env);
}

void	run_cmd(t_cmd *cmd, char **env)
{
	int	pid;

	if (!cmd)
		return ;
	receive_signal(CHILD);
	pid = fork();
	if (pid < 0)
		perror("fork");
	else if (pid == 0)
	{
		if (cmd->type == CMD_EXEC)
		{
			// Builtin checks go here
			exec_cmd(convert_cmd_exec(((t_cmd_exec *)cmd)->tokens), env);
		}
		else if (cmd->type == CMD_PIPE)
			exec_pipe((t_cmd_expr *)cmd, env);
		else if (cmd->type == CMD_AND)
			exec_pipe((t_cmd_expr *)cmd, env);
		else if (cmd->type == CMD_OR)
			exec_pipe((t_cmd_expr *)cmd, env);
		else if (cmd->type == CMD_DBL_GREAT || cmd->type == CMD_GREAT \
			|| cmd->type == CMD_LESS)
			exec_redir((t_cmd_redir *)cmd, env);
		else if (cmd->type == CMD_HEREDOC)
			exec_heredoc((t_cmd_heredoc *)cmd, env);
		exit(0);
	}
	waitpid(pid, NULL, 0);
}

/**
 * @brief Creates a child process and executes the specified command
 * Accepts an array of strings as input that will be passed to execve
 * following the format:
 * {command, options}
 */
void	exec_cmd(char **cmd, char **env)
{
	int		pid;
	char	*absolute_cmd;
	char	*cmd_original;

	cmd_original = cmd[0];
	if (!ft_strchr(cmd[0], '/'))
	{
		absolute_cmd = find_cmd(cmd[0]);
		if (!absolute_cmd)
		{
			ft_putstr_fd(cmd_original, 2);
			ft_putendl_fd(": command not found", 2);
			g_status_code = 127;
			return ;
		}
		cmd[0] = absolute_cmd;
	}
	else
	{
		if (access(cmd[0], F_OK) == -1)
		{
			ft_putstr_fd(cmd_original, 2);
			ft_putendl_fd(": no such file or directory", 2);
			g_status_code = 127;
			return ;
		}
		else if (access(cmd[0], X_OK) == -1)
		{
			ft_putstr_fd(cmd_original, 2);
			ft_putendl_fd(": permission denied", 2);
			g_status_code = 126;
			return ;
		}
	}
	receive_signal(CHILD);
	pid = fork();
	if (pid == 0)
	{
		execve(cmd[0], cmd, env);
		ft_putstr_fd(cmd_original, 2);
		ft_putendl_fd(": command not found", 2);
		free(cmd_original);
		g_status_code = 127;
		exit(127);
	}
	waitpid(pid, &g_status_code, 0);
	free(cmd_original);
	// free(absolute_cmd);
}

/**
 * @brief Checks if the command is a builtin.
 *
 * It goes through a bunch of if and else statements that call ft_strncmp()
 * with different builtin names. If one of them return a difference of 0, the
 * function returns a positive.
 *
 * @param str is the name of the command that will be compared.
 *
 * @return true if the command name is a builtin and false it it not.
 *
 */
bool	is_builtin(char *str)
{
	if (!ft_strncmp(str, "echo", 5))
		return (true);
	else if (!ft_strncmp(str, "cd", 3))
		return (true);
	else if (!ft_strncmp(str, "pwd", 4))
		return (true);
	else if (!ft_strncmp(str, "export", 7))
		return (true);
	else if (!ft_strncmp(str, "unset", 6))
		return (true);
	else if (!ft_strncmp(str, "env", 4))
		return (true);
	else if (!ft_strncmp(str, "exit", 5))
		return (true);
	return (false);
}

/**
 * @brief Executes the correct builtin.
 *
 * It goes through a bunch of if and else statements that call ft_strncmp()
 * with different builtin names. If one of them return a difference of 0, the
 * corresponding function is called.
 *
 * @param cmd contains the name of the command that will be compared.
 * @param minishell is sent to the builtins as required.
 *
 *
void	exec_builtin(char **cmd, t_minishell *minishell)
{
	if (!ft_strncmp(*cmd, "echo", 5))
		ft_echo(cmd);
	else if (!ft_strncmp(*cmd, "cd", 3))
		ft_cd(cmd, minishell);
	else if (!ft_strncmp(*cmd, "pwd", 4))
		ft_pwd(cmd);
	else if (!ft_strncmp(*cmd, "export", 7))
		ft_export(minishell, cmd);
	else if (!ft_strncmp(*cmd, "unset", 6))
		ft_unset(minishell, cmd);
	else if (!ft_strncmp(*cmd, "env", 4))
		ft_env(cmd, minishell->envp);
	// else if (!ft_strncmp(*cmd, "exit", 5))
	// 	return (true);
}
*/
