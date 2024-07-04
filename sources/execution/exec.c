/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maabdull <maabdull@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 14:47:16 by maabdull          #+#    #+#             */
/*   Updated: 2024/07/04 23:08:12 by maabdull         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
		return (ft_putendl_fd("pipe creation failed", 2));
	receive_signal(CHILD);
	pid1 = fork();
	if (pid1 < 0)
		return (ft_putendl_fd("fork failed", 2));
	if (pid1 == 0)
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
		return (ft_putendl_fd("fork failed", 2));
	if (pid2 == 0)
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

void	exec_redir(t_cmd_redir *cmd, char **env)
{
	int	fd_redirect;

	fd_redirect = 0;
	if (cmd->type == CMD_DBL_GREAT)
		fd_redirect = open(cmd->file, O_WRONLY|O_CREAT|O_APPEND, 0666);
	else if (cmd->type == CMD_GREAT)
		fd_redirect = open(cmd->file, O_WRONLY|O_CREAT|O_TRUNC, 0666);
	else if (cmd->type == CMD_LESS)
		fd_redirect = open(cmd->file, O_RDONLY);
	dup2(fd_redirect, cmd->fd);
	run_cmd(cmd->cmd, env);
}

void	run_cmd(t_cmd *cmd, char **env)
{
	t_cmd_exec	*cmd_exec;
	int	pid;

	if (!cmd)
		return ;
	receive_signal(CHILD);
	pid = fork();
	if (pid < 0)
		perror("fork failed");
	else if (pid == 0)
	{
		if (cmd->type == CMD_EXEC)
		{
			cmd_exec = (t_cmd_exec *) cmd;
			// Builtin checks go here
			exec_cmd(convert_cmd_exec(cmd_exec->tokens), env);
		}
		else if (cmd->type == CMD_PIPE)
			exec_pipe((t_cmd_expr *) cmd, env);
		else if (cmd->type == CMD_AND)
			exec_pipe((t_cmd_expr *) cmd, env);
		else if (cmd->type == CMD_OR)
			exec_pipe((t_cmd_expr *) cmd, env);
		else if (cmd->type == CMD_DBL_GREAT || cmd->type == CMD_GREAT \
			|| cmd->type == CMD_LESS)
			exec_redir((t_cmd_redir *) cmd, env);
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
	int		status;
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
			ft_putendl_fd(": Permission denied", 2);
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
	waitpid(pid, &status, 0);
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
