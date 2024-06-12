/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maabdull <maabdull@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 17:34:36 by maabdull          #+#    #+#             */
/*   Updated: 2024/06/11 21:59:35 by maabdull         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd	*create_exec_cmd(t_minishell *minishell)
{
	t_cmd_exec	*exec_cmd;
	(void) minishell;
	// t_token		*tokens;
	// int			i;

	// i = 0;
	exec_cmd = malloc(sizeof(t_cmd_exec));
	exec_cmd->type = CMD_EXEC;
	exec_cmd->tokens = NULL;
	// exec_cmd->tokens = malloc(sizeof(t_token_node));
	// while (tokens[i].content)
	// {
	// 	exec_cmd->tokens[i] = ft_strdup(tokens[i].content);
	// 	i++;
	// }
	// exec_cmd->tokens[i] = NULL;
	return ((t_cmd *) exec_cmd);
}
