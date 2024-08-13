/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maabdull <maabdull@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 17:34:36 by maabdull          #+#    #+#             */
/*   Updated: 2024/08/13 21:00:45 by maabdull         ###   ########.fr       */
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
	int		length[2];

	if (pipe(fd) < 0)
		return (-1);
	length[0] = ft_strlen(delimiter);
	while (true)
	{
		line = readline("> ");
		if (!line)
		{
			ft_putendl_fd("Warning: heredoc delimited by end-of-file", 2);
			g_code = 130;
			break ;
		}
		length[1] = ft_strlen(line);
		if (length[1] > length[0])
			length[0] = length[1];
		if (line && !ft_strncmp(line, delimiter, length[0]))
			break ;
		ft_putendl_fd(line, fd[1]);
		free(line);
	}
	return (free(line), close(fd[1]), fd[0]);
}

/**
 * @brief Create a cmd_redir struct
 *
 * @param command The command to execute
 * @param type The type of redirection this is
 * @param file The file to read from or write to
 *
 * @return the redirection command struct.
 */
t_cmd	*create_redir_cmd(t_cmd *command, t_cmd_type type, char *file, bool wc)
{
	t_cmd_redir	*redir_command;

	redir_command = ft_calloc(1, sizeof(t_cmd_redir));
	if (!redir_command)
		return (perror(file), g_code = 1, free_command(command), NULL);
	redir_command->cmd = command;
	redir_command->type = type;
	redir_command->file = file;
	redir_command->fd = -1;
	if (file && !wc && type == CMD_GREAT)
		redir_command->fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0666);
	else if (file && !wc && type == CMD_DBL_GREAT)
		redir_command->fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0666);
	else if (file && !wc && type == CMD_LESS)
		redir_command->fd = open(file, O_RDONLY);
	else if (file && !wc && type == CMD_HEREDOC)
		redir_command->fd = heredoc_creation(file);
	if (wc)
		return (ft_putstr_fd(file, 2), ft_putendl_fd(": ambiguous redirect", 2),
			g_code = 1, (t_cmd *)redir_command);
	if (redir_command->fd < 0 && file)
		return (redir_command->file = NULL, perror(file), g_code = 1, \
			(t_cmd *)redir_command);
	return ((t_cmd *)redir_command);
}

static t_cmd	*create_redir(t_cmd *command, t_tkn_type type, t_minishell *ms)
{
	char	*file;
	bool	wildcard;

	file = NULL;
	wildcard = false;
	if (!*ms->tokens->content)
		wildcard = true;
	else if (ms->tokens->id && ms->tokens->next
		&& ms->tokens->id == ms->tokens->next->id)
	{
		while (ms->tokens->next && ms->tokens->id == ms->tokens->next->id)
			ms->tokens = ms->tokens->next;
		wildcard = true;
	}
	else if (!ms->invalid)
		file = ms->tokens->content;
	if (type == LESS)
		command = create_redir_cmd(command, CMD_LESS, file, wildcard);
	else if (type == GREAT)
		command = create_redir_cmd(command, CMD_GREAT, file, wildcard);
	else if (type == DBL_GREAT)
		command = create_redir_cmd(command, CMD_DBL_GREAT, file, wildcard);
	else if (type == DBL_LESS)
		command = create_redir_cmd(command, CMD_HEREDOC, file, wildcard);
	return (command);
}

/**
 * @brief Parse a redirection command.
 *
 * This function ensures the current token and next token match the format
 * needed for redirection commands:
 * <SYMBOL> <FILE_NAME>
 * The command which is to be redirected is provided to the function as an
 * argument.

 * Starts a loop until either the tokens are over, or the current token's type
 * is a non-redirection type and performs the following actions:
 * - Check and return if the next token doesn't exist or isn't of the WORD type
 * - Check and create a redirection command struct based on the type of
 * redirection
 * Return the original or modified command struct
 *
 * @param command is the command tree
 * @param minishell is the main struct
 *
 * @return the command tree with the redirections parsed.
 */
t_cmd	*parse_redir(t_cmd *command, t_minishell *minishell)
{
	t_tkn_type	type;

	minishell->invalid = false;
	type = minishell->tokens->type;
	while (minishell->tokens && type >= LESS && type < WORD)
	{
		if (!minishell->tokens->next || minishell->tokens->next->type != WORD)
			return (ft_putendl_fd("No file for redirection found", 2),
				free_command(command), NULL);
		minishell->tokens = minishell->tokens->next;
		command = create_redir(command, type, minishell);
		if (((t_cmd_redir *)command)->fd == -1)
			minishell->invalid = true;
		minishell->tokens = minishell->tokens->next;
		if (minishell->tokens)
			type = minishell->tokens->type;
	}
	return (command);
}

/**
 * @brief Create a general expression command struct
 * These commands have a left and right side as well as an operator
 * in the middle such as AND, OR or the PIPE commands
 *
 * @param cmd_left The command to the left of the operator
 * @param cmd_right The command to the right of the operator
 *
 * @return the expression command struct.
 */
t_cmd	*create_expr_cmd(t_cmd_type type, t_cmd *cmd_left, t_cmd *cmd_right)
{
	t_cmd_expr	*expr_cmd;

	expr_cmd = ft_calloc(1, sizeof(t_cmd_expr));
	if (!expr_cmd)
		return (perror("Tokenisation"), g_code = 1, free_command(cmd_left),
			free_command(cmd_right), NULL);
	expr_cmd->type = type;
	expr_cmd->command_left = cmd_left;
	expr_cmd->command_right = cmd_right;
	return ((t_cmd *) expr_cmd);
}
