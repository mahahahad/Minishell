/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_exec.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdanish <mdanish@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 12:05:36 by mdanish           #+#    #+#             */
/*   Updated: 2024/08/02 21:55:36 by mdanish          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**convert_command(t_cmd *command, t_token *current)
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
void	exec_cmd(t_minishell *minishell, char **command, int read)
{
	int		process_id;
	int		exit_code;

	receive_signal(CHILD);
	process_id = fork();
	if (!process_id)
	{
		duplicate_fds(minishell->cmd, minishell, read);
		if (!exec_builtin(command, minishell) && \
			execve(command[0], command, minishell->envp))
			perror("execve() failed");
		free_char_command(command);
		free_parsing(minishell);
		free_environment(minishell);
		exit(WEXITSTATUS(errno));
	}
	waitpid(process_id, &exit_code, 0);
	g_code = WEXITSTATUS(exit_code);
}

void	run_command(t_minishell *minishell, int read)
{
	t_cmd	*cmd;
	char	**command;

	cmd = minishell->cmd;
	if (!cmd)
		return ;
	if ((cmd->type > CMD_PIPE && cmd->type < CMD_AND) || cmd->type == CMD_EXEC)
	{
		command = convert_command(cmd, NULL);
		if (!command)
			return ;
		is_builtin(&minishell->bltn, *command);
		if (read < 0 && ((minishell->bltn >= CD && minishell->bltn < ECHO) || \
			minishell->bltn == CD || (minishell->bltn == EXPORT && command[1])))
			exec_builtin(command, minishell);
		else if (minishell->bltn != NONE || \
			confirm_command(command, minishell->env_variables))
			exec_cmd(minishell, command, read);
		free_char_command(command);
	}
	// else
		// exec_pipe(minishell, env);
}
