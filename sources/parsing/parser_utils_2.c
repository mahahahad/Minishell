/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maabdull <maabdull@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 21:31:25 by maabdull          #+#    #+#             */
/*   Updated: 2024/08/08 21:32:46 by maabdull         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Check if the provided command tree contains a command of type 
 * CMD_AND or CMD_OR.
 * 
 * @param command 
 * @return true 
 * @return false 
 */
bool	has_expr_symbol(t_cmd *command)
{
	if (command->type == CMD_AND || \
		command->type == CMD_OR)
		return (true);
	if (command->type == CMD_GREAT || \
		command->type == CMD_DBL_GREAT || \
		command->type == CMD_LESS || \
		command->type == CMD_HEREDOC)
		return (has_expr_symbol(((t_cmd_redir *)command)->cmd));
	if (command->type == CMD_PIPE)
		return (has_expr_symbol(((t_cmd_expr *)command)->command_left) || \
			has_expr_symbol(((t_cmd_expr *)command)->command_right));
	return (false);
}
