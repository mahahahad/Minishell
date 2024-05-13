/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdanish <mdanish@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 14:47:16 by maabdull          #+#    #+#             */
/*   Updated: 2024/05/13 15:31:32 by mdanish          ###   ########.fr       */
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
void	run_cmd(t_cmd *cmd, char **env)
{
	t_cmd_exec	*cmd_exec;

	if (cmd->type == CMD_EXEC)
	{
		cmd_exec = (t_cmd_exec *) cmd;
		if (is_builtin(cmd_exec->tokens[0]))
			exec_builtin(cmd_exec->tokens);
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
			return (fprintf(stderr, "%s: command not found\n", cmd_original), 127);
		cmd[0] = absolute_cmd;
	}
	else
	{
		if (access(cmd[0], F_OK) == -1)
			return (fprintf(stderr, "%s: no such file or directory\n",
					cmd_original), 127);
		else if (access(cmd[0], X_OK) == -1)
			return (fprintf(stderr, "%s: permission denied\n", cmd_original),
				126);
	}
	pid = fork();
	if (pid == 0)
	{
		execve(cmd[0], cmd, env);
		fprintf(stderr, "%s: command not found\n", cmd_original);
		free(cmd_original);
		exit(127);
	}
	waitpid(pid, &status, 0);
	free(cmd_original);
	// free(absolute_cmd);
	return (status);
}

void	exec_builtin(char **cmd)
{
	if (ft_strncmp(cmd[0], "echo", 4) == 0)
		echo(cmd + 1);
	if (ft_strncmp(cmd[0], "cd", 2) == 0)
		g_status_code = cd(cmd + 1);
}
