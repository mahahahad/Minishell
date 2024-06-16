/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maabdull <maabdull@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 17:34:36 by maabdull          #+#    #+#             */
/*   Updated: 2024/06/16 22:36:24 by maabdull         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * This file contains constructor functions. These functions are
 * used for creating (allocating space for) various command structures
 */

/**
 * @brief Create a cmd_exec struct
 * 
 * @return t_cmd*
 */
t_cmd	*create_exec_cmd(void)
{
	t_cmd_exec	*exec_cmd;

	exec_cmd = malloc(sizeof(t_cmd_exec));
	exec_cmd->type = CMD_EXEC;
	exec_cmd->tokens = NULL;
	return ((t_cmd *) exec_cmd);
}

/**
 * @brief Create a cmd_redir struct
 * 
 * @param cmd The command to execute
 * @param type The type of redirection this is
 * @param file The file to read from or write to
 * @return t_cmd* 
 */
t_cmd	*create_redir_cmd(t_cmd *cmd, int type, char *file)
{
	t_cmd_redir	*redir_cmd;

	redir_cmd = malloc(sizeof(t_cmd_redir));
	redir_cmd->cmd = cmd;
	redir_cmd->type = type;
	redir_cmd->file = file;
	return ((t_cmd *) redir_cmd);
}
