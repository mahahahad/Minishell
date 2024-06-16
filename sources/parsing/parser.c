/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maabdull <maabdull@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 14:41:15 by maabdull          #+#    #+#             */
/*   Updated: 2024/06/16 22:41:23 by maabdull         ###   ########.fr       */
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
	if (count_quotations(line))
	{
		ft_putstr_fd(RED "Open quotes detected, command rejected.\n" RESET, 2);
		return (NULL);
	}
	minishell->tokens = tokenize(minishell, line);
	return (parse_line(minishell));
}

t_cmd	*parse_line(t_minishell *minishell)
{
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
	// Pipe handling goes here.
	if (minishell->tokens && minishell->tokens->current->type == PIPE)
 		ft_putendl_fd("PIPE HANDLING GOES HERE", 1);
	return (cmd);
}

/**
 * @brief Parsing function for ensuring that executable commands follow the proper structure
 * 
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
	parse_redir((t_cmd *) cmd, minishell);
	while (i < minishell->token_count && minishell->tokens->current->type != PIPE)
	{
		push_token(&cmd->tokens, minishell->tokens->current);
		minishell->tokens = minishell->tokens->next;
		i++;
		parse_redir((t_cmd *) cmd, minishell);
	}
	return ((t_cmd *) cmd);
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
	while (minishell->tokens && (minishell->tokens->current->type == GREAT || minishell->tokens->current->type == LESS))
	{
		if (!minishell->tokens->next || minishell->tokens->next->current->type != WORD)
			ft_putendl_fd("No file for redirection found", 1);
		if (minishell->tokens->current->type == LESS)
		{
			cmd = create_redir_cmd(cmd, LESS, minishell->tokens->next->current->content);
			break ;
		}
		else if (minishell->tokens->current->type == GREAT)
		{
			cmd = create_redir_cmd(cmd, GREAT, minishell->tokens->next->current->content);
			break ;
		}
	}
	return (cmd);
}
