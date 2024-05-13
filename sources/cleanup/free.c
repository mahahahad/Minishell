/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdanish <mdanish@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 17:38:52 by maabdull          #+#    #+#             */
/*   Updated: 2024/05/13 15:31:15 by mdanish          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_tokens(t_minishell *minishell)
{
	int	i;

	i = 0;
	while (minishell->tokens[i].content)
	{
		free(minishell->tokens[i].content);
		i++;
	}
	free(minishell->tokens);
}

void	free_cmd(t_cmd *cmd)
{
	t_cmd_exec	*cmd_exec;
	int			i;

	i = 0;
	if (cmd->type == CMD_EXEC)
	{
		cmd_exec = (t_cmd_exec *) cmd;
		while (cmd_exec->tokens[i])
		{
			free(cmd_exec->tokens[i]);
			i++;
		}
		free(cmd_exec->tokens);
		free(cmd_exec);
	}
}
