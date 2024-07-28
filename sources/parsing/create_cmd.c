/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdanish <mdanish@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 17:34:36 by maabdull          #+#    #+#             */
/*   Updated: 2024/07/20 20:32:17 by mdanish          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * This file contains constructor functions. These functions are
 * used for creating (allocating space for) various command structures
 */

/**
 * @brief Create a cmd_redir struct
 * 
 * @param cmd The command to execute
 * @param type The type of redirection this is
 * @param file The file to read from or write to
 * @return t_cmd* 
 */
t_cmd	*create_redir_cmd(t_cmd *cmd, t_cmd_type type, char *file)
{
	t_cmd_redir	*redir_cmd;

	redir_cmd = ft_calloc(1, sizeof(t_cmd_redir));
	if (!redir_cmd)
		return (perror("Redirection Command"), g_code = 1, \
			free_cmd(cmd), NULL);
	redir_cmd->cmd = cmd;
	redir_cmd->type = type;
	redir_cmd->file = file;
	if (type != CMD_LESS)
		redir_cmd->fd = STDOUT_FILENO;
	return ((t_cmd *) redir_cmd);
}

/**
 * @brief Create a general expression command struct
 * These commands have a left and right side as well as an operator
 * in the middle such as AND, OR or the PIPE commands
 * 
 * @param cmd_left The command to the left of the operator
 * @param cmd_right The command to the right of the operator
 * @return t_cmd* 
 */
t_cmd	*create_expr_cmd(t_cmd_type type, t_cmd *cmd_left, t_cmd *cmd_right)
{
	t_cmd_expr	*expr_cmd;

	expr_cmd = ft_calloc(1, sizeof(t_cmd_expr));
	if (!expr_cmd)
		return (perror("Tokenisation"), g_code = 1, free_cmd(cmd_left), \
			free_cmd(cmd_right), NULL);
	expr_cmd->type = type;
	expr_cmd->cmd_left = cmd_left;
	expr_cmd->cmd_right = cmd_right;
	return ((t_cmd *) expr_cmd);
}

t_cmd	*create_heredoc(t_cmd *cmd, char *delimiter)
{
	t_cmd_heredoc	*heredoc_cmd;

	heredoc_cmd = ft_calloc(1, sizeof(t_cmd_heredoc));
	if (!heredoc_cmd)
		return (ft_putendl_fd("Malloc failed in tokenisation.", 2), \
			g_status_code = 1, free_cmd(cmd), NULL);
	heredoc_cmd->cmd = cmd;
	heredoc_cmd->type = CMD_HEREDOC;
	heredoc_cmd->delimiter = delimiter;
	return ((t_cmd *) heredoc_cmd);
}
