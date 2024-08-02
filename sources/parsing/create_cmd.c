/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdanish <mdanish@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 17:34:36 by maabdull          #+#    #+#             */
/*   Updated: 2024/08/02 21:33:43 by mdanish          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * This file contains constructor functions. These functions are
 * used for creating (allocating space for) various command structures
 */

/**
 * @brief Create the heredoc file
 * 
 * It opens a new pipe to which return of readline are written to. Once a prompt
 * that matches the delimiter is found, it will break and return the read end of
 * the pipe to the calling function.
 * 
 * @param delimiter is the string that will terminate the document creation.
 * 
 * @return the read end of the pipe.
 */
static int	heredoc_creation(char *delimiter)
{
	char	*line;
	int		fd[2];
	int		len1;
	int		len2;

	if (pipe(fd) < 0)
		return (-1);
	len1 = ft_strlen(delimiter);
	while (true)
	{
		line = readline("> ");
		if (!line)
		{
			ft_putendl_fd("Warning: heredoc delimited by end-of-file", 2);
			g_code = 130;
			break ;
		}
		len2 = ft_strlen(line);
		if (len2 > len1)
			len1 = len2;
		if (line && !ft_strncmp(line, delimiter, len1))
			break ;
		ft_putendl_fd(line, fd[1]);
		free(line);
	}
	free(line);
	close(fd[1]);
	return (fd[0]);
}

/**
 * @brief Create a cmd_redir struct
 * 
 * @param cmd The command to execute
 * @param type The type of redirection this is
 * @param file The file to read from or write to
 * @return t_cmd* 
 */
t_cmd	*create_redir_cmd(t_cmd *command, t_cmd_type type, char *file)
{
	t_cmd_redir	*redir_cmd;

	redir_cmd = ft_calloc(1, sizeof(t_cmd_redir));
	if (!redir_cmd)
		return (perror("Redirection Command"), g_code = 1, \
			free_cmd(command), NULL);
	redir_cmd->cmd = command;
	redir_cmd->type = type;
	redir_cmd->file = file;
	redir_cmd->fd = -1;
	if (type == CMD_GREAT)
		redir_cmd->fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0666);
	else if (type == CMD_DBL_GREAT)
		redir_cmd->fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0666);
	else if (type == CMD_LESS)
		redir_cmd->fd = open(file, O_RDONLY);
	else if (type == CMD_HEREDOC)
		redir_cmd->fd = heredoc_creation(file);
	if (redir_cmd->fd < 0)
		perror("Opening a file for redirection");
	return ((t_cmd *)redir_cmd);
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

// t_cmd	*create_heredoc(t_cmd *cmd, char *delimiter)
// {
// 	t_cmd_heredoc	*heredoc_cmd;
// 
// 	heredoc_cmd = ft_calloc(1, sizeof(t_cmd_heredoc));
// 	if (!heredoc_cmd)
// 		return (ft_putendl_fd("Malloc failed in tokenisation.", 2), 
// 			g_status_code = 1, free_cmd(cmd), NULL);
// 	heredoc_cmd->cmd = cmd;
// 	heredoc_cmd->type = CMD_HEREDOC;
// 	heredoc_cmd->delimiter = delimiter;
// 	return ((t_cmd *) heredoc_cmd);
// }
