/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maabdull <maabdull@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 14:47:16 by maabdull          #+#    #+#             */
/*   Updated: 2024/06/27 19:57:25 by maabdull         ###   ########.fr       */
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
	str_tokens = malloc(sizeof(char *) * (i + 1));
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

void	exec_pipe(t_cmd_expr *cmd)
{
	t_cmd_exec *exec_cmd;

	if (cmd->cmd_left->type == CMD_EXEC)
	{
		ft_putendl_fd("Left command: ", 1);
		ft_putendl_fd("- Type: Executable", 1);
		exec_cmd = (t_cmd_exec *) cmd->cmd_left;
		while (exec_cmd->tokens)
		{
			print_token((*exec_cmd->tokens));
			exec_cmd->tokens = exec_cmd->tokens->next;
		}
	}
	else if (cmd->cmd_left->type == CMD_PIPE)
	{
		exec_pipe((t_cmd_expr *)cmd->cmd_left);
	}
	if (cmd->cmd_right->type == CMD_EXEC)
	{
		ft_putendl_fd("Right command: ", 1);
		ft_putendl_fd("- Type: Executable", 1);
		exec_cmd = (t_cmd_exec *) cmd->cmd_right;
		while (exec_cmd->tokens)
		{
			print_token((*exec_cmd->tokens));
			exec_cmd->tokens = exec_cmd->tokens->next;
		}
	}
	else if (cmd->cmd_right->type == CMD_PIPE)
	{
		exec_pipe((t_cmd_expr *)cmd->cmd_right);
	}
}

void	exec_redir(t_cmd_redir *cmd, char **env)
{
	int	fd_redirect;

	// if (cmd->type == GREAT)
	// 	fd_redirect = open(cmd->file, O_WRONLY);
	// else if (cmd->type == LESS)
	fd_redirect = open(cmd->file, O_WRONLY|O_CREAT|O_TRUNC);
	dup2(fd_redirect, 1);
	run_cmd(cmd->cmd, env);
}

void	run_cmd(t_cmd *cmd, char **env)
{
	t_cmd_exec	*cmd_exec;

	if (!cmd)
		return ;
	if (cmd->type == CMD_EXEC)
	{
		cmd_exec = (t_cmd_exec *) cmd;
		// Builtin checks go here
		exec_cmd(convert_cmd_exec(cmd_exec->tokens), env);
	}
	else if (cmd->type == CMD_PIPE)
	{
		// Pipe handling goes here
		exec_pipe((t_cmd_expr *) cmd);
	}
	else if (cmd->type == CMD_AND)
	{
		// And command handling goes here
		exec_pipe((t_cmd_expr *) cmd);
	}
	else if (cmd->type == CMD_OR)
	{
		// Or command handling goes here
		exec_pipe((t_cmd_expr *) cmd);
	}
	// Redirection handling goes here
	else if (cmd->type == CMD_LESS || cmd->type == CMD_GREAT)
	{
		exec_redir((t_cmd_redir *) cmd, env);
		// ft_putendl_fd("Redirection handling will go here.", 1);
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

// void	exec_builtin(char **cmd)
// {
// 	if (ft_strncmp(cmd[0], "echo", 5) == 0)
// 		ft_echo(cmd + 1);
// 	if (ft_strncmp(cmd[0], "cd", 3) == 0)
// 		g_status_code = ft_cd(cmd + 1);
// }
