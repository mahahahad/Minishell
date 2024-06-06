/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdanish <mdanish@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 14:47:16 by maabdull          #+#    #+#             */
/*   Updated: 2024/06/06 14:30:56 by mdanish          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

/**
 * TODO: Check if command is builtin here
 */
void	run_cmd(t_cmd *cmd, char **env, t_minishell * minishell)
{
	t_cmd_exec	*cmd_exec;

	if (cmd->type == CMD_EXEC)
	{
		cmd_exec = (t_cmd_exec *) cmd;
		if (is_builtin(cmd_exec->tokens[0]))
			exec_builtin(cmd_exec->tokens, minishell);
		else
			exec_cmd(cmd_exec->tokens, env);
	}
}

/**
 * @brief Creates a child process and executes the specified command
 * Accepts an array of strings as input that will be passed to execve
 * following the format:
 * {command, options}
 */
int	exec_cmd(char **cmd, char **env)
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
			return (127);
		}
		cmd[0] = absolute_cmd;
	}
	else
	{
		if (access(cmd[0], F_OK) == -1)
		{
			ft_putstr_fd(cmd_original, 2);
			ft_putendl_fd(": no such file or directory", 2);
			return (127);
		}
		else if (access(cmd[0], X_OK) == -1)
		{
			ft_putstr_fd(cmd_original, 2);
			ft_putendl_fd(": Permission denied", 2);
			return (126);
		}
	}
	pid = fork();
	if (pid == 0)
	{
		execve(cmd[0], cmd, env);
		ft_putstr_fd(cmd_original, 2);
		ft_putendl_fd(": command not found", 2);
		free(cmd_original);
		exit(127);
	}
	waitpid(pid, &status, 0);
	free(cmd_original);
	// free(absolute_cmd);
	return (status);
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
 */
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
