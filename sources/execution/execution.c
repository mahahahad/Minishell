/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdanish <mdanish@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 12:05:36 by mdanish           #+#    #+#             */
/*   Updated: 2024/08/04 00:18:54 by mdanish          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	**convert_command(t_cmd *command, t_token *current)
{
	char	**str_tokens;
	int		token_count[2];

	token_count[0] = 0;
	token_count[1] = 0;
	while (command->type > CMD_PIPE && command->type < CMD_AND)
		command = ((t_cmd_redir *)command)->cmd;
	current = ((t_cmd_exec *)command)->tokens;
	if (!current)
		return (NULL);
	while (current && ++token_count[0])
		current = current->next;
	str_tokens = ft_calloc(++token_count[0], sizeof(char *));
	if (!str_tokens)
		return (perror("Command conversion"), NULL);
	current = ((t_cmd_exec *)command)->tokens;
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

/**
 * @brief Creates a child process and executes the specified command
 * Accepts an array of strings as input that will be passed to execve
 * following the format:
 * {command, options}
 */
static void	exec_command(t_minishell *minishell, char **command)
{
	int		process_id;
	int		exit_code;

	process_id = fork();
	if (!process_id)
	{
		receive_signal(CHILD);
		duplicate_fds(minishell->cmd, minishell);
		if (minishell->bltn != NONE)
			execute_builtin(command, minishell);
		else if (execve(command[0], command, minishell->envp))
			perror("execve() failed");
		free_char_command(command);
		free_parsing(minishell);
		free_environment(minishell);
		exit(WEXITSTATUS(errno));
	}
	waitpid(process_id, &exit_code, 0);
	g_code = WEXITSTATUS(exit_code);
}

static void	execute_expression(t_minishell *minishell, t_cmd *command_right)
{
	t_cmd_type	type;

	type = minishell->cmd->type;
	if (type == CMD_PIPE)
	{
		if (pipe(minishell->pipe_fds) < 0)
			return (g_code = WEXITSTATUS(errno), perror("pipe creation"));
		minishell->cmd = ((t_cmd_expr *)minishell->cmd)->command_left;
		run_command(minishell, true);
		minishell->pipe_read_store = minishell->pipe_fds[0];
		minishell->cmd = command_right;
		close(minishell->pipe_fds[1]);
		minishell->pipe_fds[1] = -1;
		run_command(minishell, true);
	}
	else
	{
		minishell->cmd = ((t_cmd_expr *)minishell->cmd)->command_left;
		run_command(minishell, false);
		minishell->cmd = command_right;
		if ((type == CMD_AND && !g_code) || (type == CMD_OR && g_code))
			run_command(minishell, false);
	}
}

void	run_command(t_minishell *minishell, bool piped)
{
	t_cmd	*cmd;
	char	**command;

	cmd = minishell->cmd;
	if (!cmd)
		return ;
	if (cmd->type == CMD_PIPE || cmd->type == CMD_AND || cmd->type == CMD_OR)
		return (execute_expression(minishell, \
			((t_cmd_expr *)minishell->cmd)->command_right));
	command = convert_command(cmd, NULL);
	if (!command)
		return ;
	minishell->bltn = confirm_builtin(*command);
	if (!piped && ((minishell->bltn >= CD && minishell->bltn < ECHO) || \
		minishell->bltn == CD || (minishell->bltn == EXPORT && command[1])))
		execute_builtin(command, minishell);
	else if (minishell->bltn != NONE || \
		confirm_command(command, minishell->env_variables))
		exec_command(minishell, command);
	free_char_command(command);
	if (minishell->pipe_read_store > -1)
		close(minishell->pipe_read_store);
	minishell->pipe_read_store = -1;
}
