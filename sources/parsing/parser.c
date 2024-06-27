/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maabdull <maabdull@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 14:41:15 by maabdull          #+#    #+#             */
/*   Updated: 2024/06/27 18:16:03 by maabdull         ###   ########.fr       */
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
	minishell->tokens = tokenize(minishell, line);
	return (parse_pipe(minishell));
}

/**
 * @brief Parsing function for ensuring that pipe commands follow the proper structure
 * 
 */
t_cmd	*parse_pipe(t_minishell *minishell)
{
	t_cmd	*cmd;

	cmd = parse_exec(minishell);
	if (!cmd)
		return (NULL);
	if (minishell->tokens && minishell->tokens->type == PIPE)
	{
		minishell->tokens = minishell->tokens->next;
		cmd = create_pipe_cmd(cmd, parse_pipe(minishell));
	}
	return (cmd);
}

t_cmd	*parse_and_or(t_cmd_exec *cmd, t_minishell *minishell)
{
	if (minishell->tokens->type == AND)
	{
		if (!cmd->tokens)
			return (ft_putendl_fd("Syntax error near unexpected token `&&'", 2), NULL);
	}
	return ((t_cmd *) cmd);
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

	token_copy = malloc(sizeof(t_token));
	token_copy->next = NULL;
	token_copy->content = token->content;
	token_copy->type = token->type;
	return (token_copy);
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
	int	i;

	i = 0;
	node = create_exec_cmd();
	cmd = (t_cmd_exec *) node;
	node = parse_redir(node, minishell);
	while (minishell->tokens && minishell->tokens->type != PIPE)
	{
		parse_and_or(cmd, minishell);
		push_token(&cmd->tokens, copy_token_node(minishell->tokens));
		minishell->tokens = minishell->tokens->next;
		i++;
		node = parse_redir(node, minishell);
	}
	if (minishell->token_count && !cmd->tokens)
		return (ft_putendl_fd("Syntax error near unexpected token `|'", 2), NULL);
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
