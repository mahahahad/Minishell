/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdanish <mdanish@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 14:47:16 by maabdull          #+#    #+#             */
/*   Updated: 2024/08/02 21:57:52 by mdanish          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"

/**
 * This file contains the functions to execute the various command types
 */

// Mallocs the full path to a command
// or returns NULL if no command was found
char	*find_command(char *cmd, t_env *env)
{
	char	*final_cmd;
	char	**paths;
	int		j;

	final_cmd = NULL;
	while (env && ft_strncmp(env->key, "PATH", 5))
		env = env->next;
	if (!env)
		return (perror("Finding the command"), NULL);
	paths = ft_split(env->value, ':');
	if (!paths)
		return (perror("Finding the command"), NULL);
	j = -1;
	while (paths[++j])
	{
		final_cmd = ft_char_strjoin(paths[j], cmd, '/');
		if (!final_cmd)
			return (ft_free_2d_arr(paths), perror("Finding the command"), NULL);
		if (access(final_cmd, X_OK) == 0)
			break ;
		free(final_cmd);
		final_cmd = NULL;
	}
	return (ft_free_2d_arr(paths), final_cmd);
}

char	**convert_command(t_cmd *cmd)
{
	t_token	*current;
	char	**str_tokens;
	int		token_count[2];

	token_count[0] = 0;
	token_count[1] = 0;
	while (cmd->type > CMD_PIPE && cmd->type < CMD_AND)
		cmd = ((t_cmd_redir *)cmd)->cmd;
	current = ((t_cmd_exec *)cmd)->tokens;
	while (current && ++token_count[0])
		current = current->next;
	str_tokens = ft_calloc(++token_count[0], sizeof(char *));
	if (!str_tokens)
		return (perror("Command conversion"), NULL);
	current = ((t_cmd_exec *)cmd)->tokens;
	while (current)
	{
		str_tokens[token_count[1]] = ft_strdup(current->content);
		if (!str_tokens[token_count[1]++])
			return (free_split(str_tokens, token_count[0]), \
				perror("Command conversion"), NULL);
		current = current->next;
	}
	return (str_tokens);
}

// void	exec_pipe(t_minishell *minishell, char **env)
// {
// 	int	pid;
// 	t_cmd_expr *cmd;

// 	if (pipe(minishell->pipe_fds) < 0)
// 		return (perror("pipe"));
// 	cmd = (t_cmd_expr *)minishell->cmd;
// 	receive_signal(CHILD);
// 	pid = fork();
// 	if (pid < 0)
// 		return (perror("fork"));
// 	else if (!pid)
// 	{
// 		dup2(minishell->pipe_fds[1], STDOUT_FILENO);
// 		close(minishell->pipe_fds[0]);
// 		close(minishell->pipe_fds[1]);
// 		minishell->cmd = cmd->cmd_left;
// 		run_cmd(minishell, env);
// 		exit(0);
// 	}
// 	receive_signal(CHILD);
// 	pid = fork();
// 	if (pid < 0)
// 		return (perror("fork"));
// 	else if (!pid)
// 	{
// 		dup2(minishell->pipe_fds[0], STDIN_FILENO);
// 		close(minishell->pipe_fds[0]);
// 		close(minishell->pipe_fds[1]);
// 		minishell->cmd = cmd->cmd_right;
// 		run_cmd(minishell, env);
// 		exit(0);
// 	}
// 	close(minishell->pipe_fds[0]);
// 	close(minishell->pipe_fds[1]);
// 	waitpid(pid, NULL, 0);
// }

/**
 * @brief Check if the provided command is of a redirection type
 *
 * @param cmd
 * @return bool
 */
bool	is_redirection_cmd(t_cmd *cmd)
{
	return (cmd->type == CMD_LESS || cmd->type == CMD_HEREDOC || \
		cmd->type == CMD_GREAT || cmd->type == CMD_DBL_GREAT);
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
// t_cmd_redir	*open_files(t_cmd_redir *cmd)
// {
// 	t_cmd_redir	*final_cmd;
// 
// 	if (!is_redirection_cmd(cmd->cmd))
// 	{
// 		if (cmd->type == CMD_DBL_GREAT)
// 			open(cmd->file, O_WRONLY|O_CREAT|O_APPEND, 0666);
// 		else if (cmd->type == CMD_GREAT)
// 			open(cmd->file, O_WRONLY|O_CREAT|O_TRUNC, 0666);
// 		else if (cmd->type == CMD_LESS)
// 			open(cmd->file, O_RDONLY);
// 		return (cmd);
// 	}
// 	else
// 		final_cmd = open_files((t_cmd_redir *) cmd->cmd);
// 	if (cmd->type == CMD_DBL_GREAT)
// 		open(cmd->file, O_WRONLY|O_CREAT|O_APPEND, 0666);
// 	else if (cmd->type == CMD_GREAT)
// 		open(cmd->file, O_WRONLY|O_CREAT|O_TRUNC, 0666);
// 	else if (cmd->type == CMD_LESS)
// 		open(cmd->file, O_RDONLY);
// 	return (final_cmd);
// }

/**
 * @brief Execute a redirection command.
 *
 * This function is run when a redirection command is encountered.
 * It opens the file specified in the command and uses that for redirection
 *
 * Creates an empty redirection command struct to save the first command in.
 * Opens all the files it finds in the redirection tree using the open_files
 * function. Checks and opens the first commands file with the appropriate
 * parameters. dup2's the opened file descriptor to the specified file
 * descriptor of the final command. Executes the command stored in the final
 * commands command variable using the run_cmd function.
 *
 * @param cmd
 * @param env
 */
// void	exec_redir(t_minishell *minishell, t_cmd_redir *cmd, char **env)
// {
// 	int	fd_redirect;
// 	t_cmd_redir *cmd_head;
// 
// 	fd_redirect = 0;
// 	cmd_head = cmd;
// 	cmd = open_files(cmd);
// 	if (cmd_head->type == CMD_DBL_GREAT)
// 		fd_redirect = open(cmd_head->file, O_WRONLY|O_CREAT|O_APPEND, 0666);
// 	else if (cmd_head->type == CMD_GREAT)
// 		fd_redirect = open(cmd_head->file, O_WRONLY|O_CREAT|O_TRUNC, 0666);
// 	else if (cmd_head->type == CMD_LESS)
// 		fd_redirect = open(cmd_head->file, O_RDONLY);
// 	dup2(fd_redirect, cmd->stdfd);
// 	run_cmd(minishell, cmd->cmd, env);
// }

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
// void	exec_heredoc(t_minishell *minishell, t_cmd_heredoc *cmd, char **env)
// {
// 	char	*line;
// 	int		fd[2];
// 
// 	if (pipe(fd) < 0)
// 		perror("pipe");
// 	ft_putendl_fd("Running heredoc", 1);
// 	while (true)
// 	{
// 		line = readline("> ");
// 		if (line && !ft_strncmp(line, cmd->delimiter, 
// 			get_longer_length(line, cmd->delimiter)))
// 			break ;
// 		ft_putendl_fd(line, fd[1]);
// 		free(line);
// 	}
// 	free(line);
// 	dup2(fd[0], STDIN_FILENO);
// 	close(fd[0]);
// 	close(fd[1]);
// 	run_cmd(minishell, cmd->cmd, env);
// }

/**
 * @brief Creates a child process and executes the specified command
 * Accepts an array of strings as input that will be passed to execve
 * following the format:
 * {command, options}
 */
// void	exec_cmd(t_minishell *minishell, char **cmd)
// {
// 	int		pid;
// 	char	*cmd_original;
// 	int		exit_code;

// 	// Builtin checks go here;
// 	if (!ft_strchr(cmd[0], '/'))
// 	{
// 		cmd_original = cmd[0];
// 		cmd[0] = find_command(cmd[0], minishell->env_variables);
// 		if (!cmd[0])
// 			return (g_code = 127, *cmd = cmd_original, ft_putstr_fd(*cmd, 2), 
// 			free_char_command(cmd), ft_putendl_fd(": command not found", 2));
// 	}
// 	else
// 	{
// 		if (access(cmd[0], F_OK) == -1)
// 			return (ft_putstr_fd(cmd[0], 2), free_char_command(cmd), 
// 				g_code = 127, ft_putendl_fd(": no such file or directory", 2));
// 		else if (access(cmd[0], X_OK) == -1)
// 			return (ft_putstr_fd(cmd[0], 2), free_char_command(cmd), 
// 				g_code = 126, ft_putendl_fd(": permission denied", 2));
// 	}
// 	receive_signal(CHILD);
// 		pid = fork();
// 	if (pid == 0)
// 	{
// 		execve(cmd[0], cmd, minishell->envp);
// 		perror("execve() failed");
// 		free_char_command(cmd);
// 		free_parsing(minishell);
// 		exit(WEXITSTATUS(errno));
// 	}
// 	waitpid(pid, &exit_code, 0);
// 	free_char_command(cmd);
// 	g_code = WEXITSTATUS(exit_code);
// }

// void	run_cmd(t_minishell *minishell, char **env)
// {
// 	t_cmd	*cmd;

// 	cmd = minishell->cmd;
// 	if (!cmd)
// 		return ;
// 	receive_signal(CHILD);
// 	if (cmd->type == CMD_EXEC)
// 		exec_cmd(minishell, convert_command(cmd));
// 	else if (cmd->type == CMD_PIPE)
// 		exec_pipe(minishell, env);
// 	else if (cmd->type == CMD_AND)
// 		exec_pipe(minishell, env);
// 	else if (cmd->type == CMD_OR)
// 		exec_pipe(minishell, env);
// 	else if (cmd->type == CMD_DBL_GREAT || cmd->type == CMD_GREAT 
// 		|| cmd->type == CMD_LESS)
// 		exec_redir(minishell, (t_cmd_redir *)cmd, env);
// 	else if (cmd->type == CMD_HEREDOC)
// 		exec_heredoc(minishell, (t_cmd_heredoc *)cmd, env);
// }

/**
 * @brief Duplicates the fds as necessary.
 * 
 * The function goes through the command tree and identifies the first instance
 * of a redirection and duplicates it. It duplicates to the STDOUT or STDIN on
 * the basis of the command type. While it goes through the tree of the
 * redirections, it closes every single open fd to prevent fd leaks.
 * 
 * @param cmd is the tree to identify redirection from.
 * @param minishell contains flags for the duplication.
 * @param read is a flag to indicate which end of the pipe to duplicate.
 */
// void	duplicate_fds(t_cmd	*cmd, t_minishell *minishell, bool read)
// {
// 	while (cmd->type == CMD_DBL_GREAT || cmd->type == CMD_GREAT || 
// 		cmd->type == CMD_LESS || cmd->type == CMD_HEREDOC)
// 	{
// 		if ((cmd->type == CMD_GREAT || cmd->type == CMD_DBL_GREAT) && 
// 			!minishell->output_fd && dup2(((t_cmd_redir *)cmd)->fd, 1))
// 			minishell->output_fd = true;
// 		else if ((cmd->type == CMD_LESS || cmd->type == CMD_HEREDOC) && 
// 			!minishell->input_fd && dup2(((t_cmd_redir *)cmd)->fd, 0))
// 			minishell->input_fd = true;
// 		if (((t_cmd_redir *)cmd)->fd > -1)
// 			close(((t_cmd_redir *)cmd)->fd);
// 		cmd = ((t_cmd_redir *)cmd)->cmd;
// 	}
// 	if (!minishell->output_fd && !read && minishell->pipe_fds[1] > -1)
// 		dup2(minishell->pipe_fds[1], STDOUT_FILENO);
// 	if (!minishell->input_fd && read && minishell->pipe_read_store > -1)
// 		dup2(minishell->pipe_read_store, STDIN_FILENO);
// 	if (minishell->pipe_fds[0] > -1)
// 		close(minishell->pipe_fds[0]);
// 	if (minishell->pipe_fds[1] > -1)
// 		close(minishell->pipe_fds[1]);
// 	if (minishell->pipe_read_store > -1)
// 		close(minishell->pipe_read_store);
// }

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
// bool	is_builtin(t_bltn *builtin, char *str)
// {
// 	if (!ft_strncmp(str, "cd", 3))
// 		return (*builtin = CD, true);
// 	else if (!ft_strncmp(str, "echo", 5))
// 		return (*builtin = ECHO, true);
// 	else if (!ft_strncmp(str, "env", 4))
// 		return (*builtin = ENV, true);
// 	else if (!ft_strncmp(str, "exit", 5))
// 		return (*builtin = EXIT, true);
// 	else if (!ft_strncmp(str, "export", 7))
// 		return (*builtin = EXPORT, true);
// 	else if (!ft_strncmp(str, "pwd", 4))
// 		return (*builtin = PWD, true);
// 	else if (!ft_strncmp(str, "unset", 6))
// 		return (*builtin = UNSET, true);
// 	return (*builtin = NONE, false);
// }

/**
 * @brief Executes the correct builtin.
 *
 * It goes through a bunch of if and else statements that call ft_strncmp()
 * with different builtin names. If one of them return a difference of 0, the
 * corresponding function is called.
 *
 * @param cmd contains the name of the command that will be compared.
 * @param minishell is sent to the builtins as required.
bool	exec_builtin(char **cmd, t_minishell *minishell)
{
	if (minishell->builin == CD)
		return (ft_cd(cmd, minishell), true);
	else if (minishell->builin == ECHO)
		return (ft_echo(cmd), true);
	else if (minishell->builin == ENV)
		return (ft_env(cmd, minishell->envp), true);
	// else if (minishell->builin_command == EXIT)
	// 	return (ft_exit(cmd, minishell), true);
	else if (minishell->builin == EXPORT)
		return (ft_export(minishell, cmd), true);
	else if (minishell->builin == PWD)
		return (ft_pwd(cmd), true);
	else if (minishell->builin == UNSET)
		return (ft_unset(minishell, cmd), true);
	return (false);
}
*/
