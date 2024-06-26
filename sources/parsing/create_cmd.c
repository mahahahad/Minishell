/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdanish <mdanish@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 17:34:36 by maabdull          #+#    #+#             */
/*   Updated: 2024/06/26 16:40:25 by mdanish          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd	*create_exec_cmd(t_minishell *minishell)
{
	t_cmd_exec	*exec_cmd;
	t_token		*tokens;
	int			i;

	i = 0;
	exec_cmd = ft_calloc(1, sizeof(t_cmd_exec));
	exec_cmd->type = CMD_EXEC;
	exec_cmd->tokens = ft_calloc((minishell->token_count + 1), sizeof(char *));
	tokens = minishell->tokens;
	while (tokens[i].content)
	{
		exec_cmd->tokens[i] = ft_strdup(tokens[i].content);
		i++;
	}
	exec_cmd->tokens[i] = NULL;
	return ((t_cmd *) exec_cmd);
}
