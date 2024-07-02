/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maabdull <maabdull@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 14:41:15 by maabdull          #+#    #+#             */
/*   Updated: 2024/07/02 12:21:48 by maabdull         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * This file contains functions directly used for parsing commands
 */

/**
 * @brief General parsing function that is called after receiving user input
 * Goes through the input and parses it by following the grammar.
 *
 * @return t_cmd*
 */
t_cmd	*parse(t_minishell *minishell, char *line)
{
	if (!line || !line[0])
		return (NULL);
	if (count_quotations(line))
	{
		ft_putstr_fd(RED "Open quotes detected, command rejected.\n" RESET, 2);
		return (NULL);
	}
	tokenize(minishell, line);
	return (parse_expr(minishell));
}

/**
 * @brief General expression parser. Checks if the current token follows the
 * proper grammar for a command which is stopped when all the tokens are
 * finished or a command delimiter has been found. If there are still tokens
 * that remain, this function creates an expression from them as the format for
 * all the delimiters is the same with them having a left and right side and an * operator.
 *
 * @param minishell
 * @return t_cmd*
 */
t_cmd	*parse_expr(t_minishell *minishell)
{
	t_cmd	*cmd;

	cmd = parse_exec(minishell);
	if (!cmd)
		return (NULL);
	if (!minishell->tokens)
		return (cmd);
	if (minishell->tokens->type == PIPE)
	{
		minishell->tokens = minishell->tokens->next;
		cmd = create_expr_cmd(CMD_PIPE, cmd, parse_expr(minishell));
	}
	else if (minishell->tokens->type == AND)
	{
		minishell->tokens = minishell->tokens->next;
		cmd = create_expr_cmd(CMD_AND, cmd, parse_expr(minishell));
	}
	else if (minishell->tokens->type == OR)
	{
		minishell->tokens = minishell->tokens->next;
		cmd = create_expr_cmd(CMD_OR, cmd, parse_expr(minishell));
	}
	return (cmd);
}

/**
 * @brief Allocates space for, and copies, only the first node of the provided
 * token node. Useful for copying tokens into a command struct without it's
 * next values.
 *
 * @param token
 * @return t_token*
 */
t_token	*copy_token_node(t_token *token)
{
	t_token	*token_copy;

	token_copy = ft_calloc(1, sizeof(t_token));
	token_copy->next = NULL;
	token_copy->content = token->content;
	token_copy->type = token->type;
	return (token_copy);
}

/**
 * @brief Utility function to display error messages when the grammar rules
 * for an execution command aren't met.
 *
 * @param minishell
 * @return bool is_error which is true if an error occured and false otherwise.
 */
bool	contains_grammar_error(t_minishell *minishell)
{
	bool	is_error;

	is_error = false;
	if (minishell->tokens->type == PIPE && (is_error = true))
		ft_putendl_fd("Syntax error near unexpected token `|'", 2);
	if (minishell->tokens->type == AND && (is_error = true))
		ft_putendl_fd("Syntax error near unexpected token `&&'", 2);
	if (minishell->tokens->type == OR && (is_error = true))
		ft_putendl_fd("Syntax error near unexpected token `||'", 2);
	if (is_error)
		g_status_code = 2;
	return (is_error);
}

/**
 * @brief Utility function which checks if the current token type should break
 * the execution parsing loop. i.e. is it a PIPE, AND or OR token
 *
 * @param minishell
 * @return true
 * @return false
 */
bool	is_exec_delimiter(t_minishell *minishell)
{
	if (minishell->tokens->type == PIPE || \
		minishell->tokens->type == AND || \
		minishell->tokens->type == OR)
		return (true);
	return (false);
}

/**
 * @brief Parsing function for ensuring that executable commands follow the proper structure
 *
 * Prints the appropriate error message if the syntax isn't met such as
 * providing special characters at the beginning of the command and returns
 * NULL.
 * @param minishell
 * @return t_cmd*
 */
t_cmd	*parse_exec(t_minishell *minishell)
{
	t_cmd	*node;
	t_cmd_exec	*cmd;

	node = create_exec_cmd();
	cmd = (t_cmd_exec *) node;
	node = parse_redir(node, minishell);
	while (minishell->tokens)
	{
		if (!cmd->tokens)
			if (contains_grammar_error(minishell))
				return (NULL);
		if (is_exec_delimiter(minishell))
			break ;
		push_token(&cmd->tokens, copy_token_node(minishell->tokens));
		minishell->tokens = minishell->tokens->next;
		node = parse_redir(node, minishell);
	}
	return (node);
}

/**
 * @brief Parsing function for ensuring that redirection commands follow the proper structure
 *
 * @param cmd
 * @param minishell
 * @return t_cmd*
 */
t_cmd	*parse_redir(t_cmd *cmd, t_minishell *minishell)
{
	while (minishell->tokens && (minishell->tokens->type == GREAT || minishell->tokens->type == LESS))
	{
		if (!minishell->tokens->next || minishell->tokens->next->type != WORD)
			ft_putendl_fd("No file for redirection found", 1);
		if (minishell->tokens->type == LESS)
		{
			cmd = create_redir_cmd(cmd, CMD_LESS, minishell->tokens->next->content);
			minishell->tokens = minishell->tokens->next->next;
			break ;
		}
		else if (minishell->tokens->type == GREAT)
		{
			cmd = create_redir_cmd(cmd, CMD_GREAT, minishell->tokens->next->content);
			minishell->tokens = minishell->tokens->next->next;
			break ;
		}
	}
	return (cmd);
}
