/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myousaf <myousaf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 12:05:36 by mdanish           #+#    #+#             */
/*   Updated: 2024/08/15 14:08:15 by myousaf          ###   ########.fr       */
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
			return (free_split(str_tokens, token_count[0]),
				perror("Command conversion"), NULL);
		current = current->next;
	}
	return (str_tokens);
}

static void	change_shlvl(t_minishell *minishell)
{
	int	index[2];

	*index = 0;
	while (minishell->envp[*index] && \
		ft_strncmp(minishell->envp[*index], "SHLVL", 6) != '=')
		(*index)++;
	if (!minishell->envp[*index])
		return ;
	index[1] = 0;
	while (minishell->envp[*index][index[1] + 6] && \
		ft_isdigit(minishell->envp[*index][index[1] + 6]))
		index[1]++;
	if (minishell->envp[*index][index[1] + 6])
		return ((void)ft_memcpy(minishell->envp[*index], "SHLVL=1", 8));
	index[1] = ft_atoi(minishell->envp[*index] + 6) + 1;
	if (index[1] > 1024)
	{
		ft_putstr_fd("warning: shell level (", 2);
		ft_putnbr_fd(index[1], 2);
		ft_putendl_fd(") too high, resetting to 1", 2);
		return ((void)ft_memcpy(minishell->envp[*index], "SHLVL=1", 8));
	}
	free(minishell->envp[*index]);
	minishell->envp[*index] = ft_strjoin_free("SHLVL=", ft_itoa(index[1]), 2);
}

/**
 * @brief Creates a child process and executes the specified command
 * Accepts an array of strings as input that will be passed to execve
 * following the format:
 * {command, options}
 */
static void	exec_command(t_minishell *minishell, char **command)
{
	int	code;
	int	i;

	receive_signal(CHILD);
	minishell->process_id = fork();
	if (!minishell->process_id)
	{
		if (duplicate_fds(minishell->cmd, minishell))
		{
			if (ft_strnstr(*command, "minishell", 10))
				change_shlvl(minishell);
			if (minishell->bltn != NONE)
				execute_builtin(command, minishell);
			else if (execve(command[0], command, minishell->envp))
				perror("execve() failed");
		}
		i = 0;
		while (i < 1025)
			close(i++);
		return (free_char_command(command), free_parsing(minishell), \
			free_environment(minishell), exit(g_code));
	}
	if (!minishell->piped && waitpid(minishell->process_id, &code, 0) && \
		WIFEXITED(code))
		g_code = WEXITSTATUS(code);
}

static void	execute_expression(t_minishell *minishell, t_cmd *command_right, \
	int code)
{
	t_cmd_type	type;

	type = minishell->cmd->type;
	if (type == CMD_PIPE)
	{
		if (pipe(minishell->pipe_fds) < 0)
			return (g_code = 13, perror("pipe creation"));
		minishell->cmd = ((t_cmd_expr *)minishell->cmd)->command_left;
		minishell->piped = true;
		run_command(minishell);
		minishell->pipe_read_store = minishell->pipe_fds[0];
		minishell->cmd = command_right;
		run_command(minishell);
		if (minishell->piped && waitpid(minishell->process_id, &code, 0) && \
			WIFEXITED(code))
			g_code = WEXITSTATUS(code);
		return ((void)(minishell->piped = false));
	}
	minishell->cmd = ((t_cmd_expr *)minishell->cmd)->command_left;
	(run_command(minishell), minishell->cmd = command_right);
	if (minishell->cmd->type != CMD_PIPE && 
		waitpid(minishell->process_id, &code, 0) && WIFEXITED(code))
		g_code = WEXITSTATUS(code);
	if ((type == CMD_AND && !g_code) || (type == CMD_OR && g_code))
		run_command(minishell);
}

void	run_command(t_minishell *minishell)
{
	t_cmd	*cmd;
	char	**command;

	cmd = minishell->cmd;
	if (!cmd)
		return ;
	if (cmd->type == CMD_PIPE || cmd->type == CMD_AND || cmd->type == CMD_OR)
		return (execute_expression(minishell,
				((t_cmd_expr *)minishell->cmd)->command_right, 0));
	command = convert_command(cmd, NULL);
	if (!command)
		return ;
	minishell->bltn = confirm_builtin(*command);
	if (!minishell->piped && ((minishell->bltn >= CD && minishell->bltn < ECHO) \
		|| minishell->bltn == CD || (minishell->bltn == EXPORT && command[1])))
		execute_builtin(command, minishell);
	else if (minishell->bltn != NONE || \
		confirm_command(command, minishell->env_variables))
		exec_command(minishell, command);
	free_char_command(command);
	if (minishell->pipe_read_store > -1 && !close(minishell->pipe_read_store))
		minishell->pipe_read_store = -1;
	if (minishell->pipe_fds[1] > -1 && !close(minishell->pipe_fds[1]))
		minishell->pipe_fds[1] = -1;
}
