/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdanish <mdanish@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 19:25:06 by mdanish           #+#    #+#             */
/*   Updated: 2024/08/09 00:45:41 by mdanish          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Duplicates the fds as necessary.
 * 
 * The function goes through the command tree and identifies the first instance
 * of a redirection and duplicates it. It duplicates to the STDOUT or STDIN on
 * the basis of the redirection type. While it goes through the tree of the
 * redirections, it closes every single open fd to prevent fd leaks.
 * 
 * @param command is the tree to identify redirection from.
 * @param minishell contains flags for the duplication.
 * @param read is a flag to indicate which end of the pipe to duplicate.
 */
void	duplicate_fds(t_cmd	*command, t_minishell *minishell)
{
	while (command->type > CMD_PIPE && command->type < CMD_AND)
	{
		if ((command->type == CMD_GREAT || command->type == CMD_DBL_GREAT)
			&& !minishell->output_fd && ((t_cmd_redir *)command)->fd > -1
			&& dup2(((t_cmd_redir *)command)->fd, 1))
			minishell->output_fd = true;
		else if ((command->type == CMD_LESS || command->type == CMD_HEREDOC)
			&& !minishell->input_fd && ((t_cmd_redir *)command)->fd > -1
			&& !dup2(((t_cmd_redir *)command)->fd, 0))
			minishell->input_fd = true;
		if (((t_cmd_redir *)command)->fd > -1)
			close(((t_cmd_redir *)command)->fd);
		command = ((t_cmd_redir *)command)->cmd;
	}
	if (!minishell->output_fd && minishell->pipe_fds[1] > -1)
		dup2(minishell->pipe_fds[1], STDOUT_FILENO);
	if (!minishell->input_fd && minishell->pipe_read_store > -1)
		dup2(minishell->pipe_read_store, STDIN_FILENO);
	if (minishell->pipe_fds[0] > -1)
		close(minishell->pipe_fds[0]);
	if (minishell->pipe_fds[1] > -1)
		close(minishell->pipe_fds[1]);
	if (minishell->pipe_read_store > -1)
		close(minishell->pipe_read_store);
	minishell->cmd = command;
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
t_bltn	confirm_builtin(char *command)
{
	if (!ft_strncmp(command, "cd", 3))
		return (CD);
	else if (!ft_strncmp(command, "echo", 5))
		return (ECHO);
	else if (!ft_strncmp(command, "env", 4))
		return (ENV);
	else if (!ft_strncmp(command, "exit", 5))
		return (EXIT);
	else if (!ft_strncmp(command, "export", 7))
		return (EXPORT);
	else if (!ft_strncmp(command, "pwd", 4))
		return (PWD);
	else if (!ft_strncmp(command, "unset", 6))
		return (UNSET);
	return (NONE);
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
*/
void	execute_builtin(char **command, t_minishell *minishell)
{
	if (minishell->bltn == CD)
		ft_cd(command, minishell);
	else if (minishell->bltn == ECHO)
		ft_echo(command);
	else if (minishell->bltn == ENV)
		ft_env(command, minishell->envp);
	else if (minishell->bltn == EXIT)
		ft_exit(command, minishell);
	else if (minishell->bltn == EXPORT)
		ft_export(minishell, command);
	else if (minishell->bltn == PWD)
		ft_pwd(command);
	else if (minishell->bltn == UNSET)
		ft_unset(minishell, command);
}

static char	*find_command(char *command, t_env *environment)
{
	char	*final_command;
	char	**paths;
	int		index;

	final_command = NULL;
	while (environment && ft_strncmp(environment->key, "PATH", 5))
		environment = environment->next;
	if (!environment)
		return (NULL);
	paths = ft_split(environment->value, ':');
	if (!paths)
		return (perror("Finding the command"), NULL);
	index = -1;
	while (paths[++index])
	{
		final_command = ft_char_strjoin(paths[index], command, '/');
		if (!final_command)
			return (ft_free_2d_arr(paths), perror("Finding the command"), NULL);
		if (!access(final_command, X_OK))
			break ;
		free(final_command);
		final_command = NULL;
	}
	return (ft_free_2d_arr(paths), final_command);
}

bool	confirm_command(char **cmd, t_env *environment)
{
	char	*cmd_original;
	t_stat	stat_buffer;

	if (!ft_strchr(*cmd, '/'))
	{
		cmd_original = *cmd;
		*cmd = find_command(*cmd, environment);
		if (!*cmd)
			return (ft_putstr_fd(cmd_original, 2), *cmd = cmd_original, \
				g_code = 127, ft_putendl_fd(": command not found", 2), false);
		return (free(cmd_original), true);
	}
	stat(*cmd, &stat_buffer);
	if (S_ISDIR(stat_buffer.st_mode))
		return (ft_putstr_fd(*cmd, 2), g_code = 126,
			ft_putendl_fd(": Is a directory", 2), false);
	else if (!S_ISREG(stat_buffer.st_mode))
			return (ft_putstr_fd(*cmd, 2), g_code = 127,
				ft_putendl_fd(": No such file or directory", 2), false);
	else if (!(stat_buffer.st_mode & S_IXUSR))
			return (ft_putstr_fd(*cmd, 2), g_code = 126,
				ft_putendl_fd(": Permission denied", 2), false);
	return (true);
}
