/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maabdull <maabdull@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 14:41:15 by maabdull          #+#    #+#             */
/*   Updated: 2024/06/15 20:44:12 by maabdull         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include <stdbool.h>

int	count_quotations(char *line)
{
	int		count;
	int		i;
	char	quotes_found;

	i = -1;
	count = 0;
	quotes_found = '\0';
	while (line[++i])
	{
		if (line[i] == '"')
		{
			if (!quotes_found && ++count)
				quotes_found = '"';
			else if (quotes_found == '"' && ++count)
				quotes_found = '\0';
		}
		if (line[i] == '\'')
		{
			if (!quotes_found && ++count)
				quotes_found = '\'';
			else if (quotes_found == '\'' && ++count)
				quotes_found = '\0';
		}
	}
	return (count % 2);
}

/**
 * @brief
 * Check if the provided character is in the list of special characters:
 * - [ | ]
 * - [ ( ]
 * - [ ) ]
 * - [ < ]
 * - [ > ]
 * - [ || ]
 * - [ << ]
 * - [ >> ]
 * - [ && ]
 */
bool	is_delimiter(char c)
{
	if (c && ft_strchr("|<>()&", c))
		return (true);
	return (false);
}

/** @brief
 * Checks if a character is in the list of repeatable characters:
 * - [<<]
 * - [>>]
 * - [||]
 * - [&&]
 */
bool	is_repeatable_char(char c1, char c2)
{
	if ((c1 == '<' && c2 == '<')\
			|| (c1 == '>' && c2 == '>')\
			|| (c1 == '|' && c2 == '|')\
			|| (c1 == '&' && c2 == '&'))
		return (true);
	return (false);
}

/**
 * @brief
 * Returns the next token in the provided input string.
 * Skip the leading whitespace characters by moving the string head
 * pointer ahead.
 * Move the string head pointer to the start of the next token
 * or '\0' if there isn't one and return this token.
 *
 * @usage
 *
 * Calling the function with the input:
 * - '       This|token '
 *
 * will result in the function returning "This" and input pointing to '|'
 *
 * Calling the function with the input:
 * - 'token'
 *
 * will result in the function returning "token" and input pointing to '\0'.
 *
 * Calling the function with the input:
 * - 'token$HOME'
 *
 * will result in the function returning "token/home/user" and input pointing to '\0'.
 */
char	*get_token(char **input)
{
	int		i;
	char	*token;
	char	*string;
	char	quotes_found;

	i = -1;
	token = NULL;
	string = *input;
	quotes_found = '\0';
	while (ft_isspace(*string))
		string++;
	while (string[++i])
	{
		// if (quotes_found != '\'' && string[i] == '$')
		// 	get_env_var(string, &i);
		// if (!string[i])
		// 	break ;
		if (ft_is_quotation(string[i]) && (quotes_found = string[i]))
			continue ;
		if (is_delimiter(string[i]))
		{
			if (is_repeatable_char(string[i], string[i + 1]))
			{
				if (!i)
					i += 2;
				break ;
			}
			else
			{
				if (!i)
					i++;
				break ;
			}
		}
		if (ft_isspace(string[i]))
		{
			if (!i)
				i++;
			break ;
		}
	}
	token = malloc(i + 1);
	if (!token)
		return (NULL);
	ft_strlcpy(token, string, i + 1);
	string += i;
	while (ft_isspace(*string))
		string++;
	*input = string;
	return (token);
}

int	get_token_type(char *content)
{
	if (!content || !content[0])
		return (ERR);
	if (!content[1])
	{
		if (content[0] == '|')
			return (PIPE);
		if (content[0] == '>')
			return (GREAT);
		if (content[0] == '<')
			return (LESS);
	}
	else if (!content[2])
	{
		if (content[0] == '>' && content[1] == '>')
			return (DBL_GREAT);
		if (content[0] == '<' && content[1] == '<')
			return (DBL_LESS);
		if (content[0] == '|' && content[1] == '|')
			return (OR);
		if (content[0] == '&' && content[1] == '&')
			return (AND);
	}
	return (WORD);
}

/**
 * @brief
 * Counts how many tokens there are in the provided input string.
 * Uses spaces and special chars as delimiters but ignores them in quoted
 * strings.
 */
int	count_tokens(char *input)
{
	int		i;
	int		token_count;

	i = -1;
	token_count = 0;
	while (ft_isspace(*input))
		input++;
	while (input[++i])
	{
		if (ft_is_quotation(input[i]))
			continue ;
		if (ft_isspace(input[i]) && input[i + 1])
		{
			token_count++;
			token_count += count_tokens(input + i + 1);
			break ;
		}
		if (is_delimiter(input[i]))
		{
			if (i > 0 && !ft_isspace(input[i - 1]))
				token_count++;
			token_count++;
			if (is_repeatable_char(input[i], input[i + 1]))
				i++;
			token_count += count_tokens(input + i + 1);
			break ;
		}
	}
	if (i > 0 && !token_count)
		token_count++;
	return (token_count);
}

/**
 * @brief Add the specified token to the end of the tokens list
 */
void	push_token(t_token_node **tokens, t_token *token)
{
	t_token_node	*new_token;
	t_token_node	*current;

	new_token = malloc(sizeof(t_token_node));
	new_token->current = token;
	new_token->next = NULL;
	current = (*tokens);
	if (!current)
	{
		(*tokens) = new_token;
		return ;
	}
	while (current->next)
		current = current->next;
	current->next = new_token;
	// return (tokens_head);
}

/**
 * @brief
 * Split the input based on all the tokens
 * Sets the tokens linked list in the minishell struct to allow for convenient navigation in other functions
 *
 * @returns
 * The linked list containing all the tokens
*/
t_token_node	*tokenize(t_minishell *minishell, char *input)
{
	t_token	*token;
	t_token_node	*tokens_list;
	int		i;
	int		token_count;

	i = 0;
	token_count = count_tokens(input);
	minishell->token_count = token_count;
	tokens_list = NULL;
	ft_putstr_fd("There are ", 1);
	ft_putnbr_fd(token_count, 1);
	ft_putendl_fd(" tokens in your input", 1);
	while (i < token_count)
	{
		token = malloc(sizeof(t_token));
		token->content = get_token(&input);
		token->type = get_token_type(token->content);
		push_token(&tokens_list, token);
		// tokens_list->current = token;
		// tokens_list->next = NULL;
		// tokens_list = tokens_list->next;
		// tokens_list = malloc(sizeof(t_token_node));
		i++;
	}
	minishell->tokens_head = &tokens_list;
	// minishell->active_token = &tokens[0];
	return (tokens_list);
}

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

t_cmd	*parse_pipe(t_minishell *minishell)
{
	t_cmd	*cmd;

	cmd = parse_exec(minishell);
	// Pipe handling goes here.
	if (minishell->tokens && minishell->tokens->current->type == PIPE)
 		puts("PIPE HANDLING GOES HERE");
	return (cmd);
}


t_cmd	*parse_exec(t_minishell *minishell)
{
	t_cmd	*node;
	t_cmd_exec	*cmd;
	// t_token_node	*cmd_tokens;
	// t_token_node	cmd_tokens_head;
	int	i;

	i = 0;
	node = create_exec_cmd(minishell);
	cmd = (t_cmd_exec *) node;
	// parse_redir(minishell);
	// cmd_tokens = cmd->tokens;
	// cmd_tokens = malloc(sizeof(t_token_node));
	// cmd_tokens_head = *cmd_tokens;
	while (i < minishell->token_count && minishell->tokens->current->type != PIPE)
	{
		push_token(&cmd->tokens, minishell->tokens->current);
		minishell->tokens = minishell->tokens->next;
		i++;
		// cmd_tokens->current = minishell->active_token;
		// // cmd->tokens->current = minishell->active_token;
		// minishell->active_token = malloc(sizeof(t_token_node));
		// cmd_tokens = cmd_tokens->next;
		// cmd_tokens = malloc(sizeof(t_token_node));
	}
	// cmd->tokens = &cmd_tokens_head;
	return ((t_cmd *) cmd);
}

// t_cmd	*parse_redir(t_minishell *minishell)
// {
// 	while (minishell->tokens->current->type == GREAT || minishell->tokens->current->type == LESS)
// 	{
// 		minishell->tokens = minishell->tokens->next;
// 		if (!minishell->tokens || minishell->tokens->current->type != WORD)
// 			puts("No file for redirection found");
// 	}
// }

/*
Logic

Tokens are a linked list
Current token is a t_token
Next token is a t_token_node
Another variable stores the head of the linked list (in case it is needed)

Looping through while creating cmd_exec iterates through the tokens linked list and updates the first token in the linked list
First token in the linked list is used for comparisons in parsing
*/
