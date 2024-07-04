/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maabdull <maabdull@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/16 22:36:59 by maabdull          #+#    #+#             */
/*   Updated: 2024/07/04 14:07:12 by maabdull         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * This file contains the utility functions used for the main parsing functions
 */

/**
 * @brief Count the number of quotation pairs in the provided line
 */
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
	token = ft_calloc(i + 1, sizeof(char));
	if (!token)
		return (NULL);
	ft_strlcpy(token, string, i + 1);
	string += i;
	while (ft_isspace(*string))
		string++;
	*input = string;
	return (token);
}

/**
 * @brief Identify the type of the token passed using the content parameter
 *
 * Returns word by default if the content parameter doesn't satisfy any of the requirements
 * @return int from e_token_types
 */
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
void	push_token(t_token **tokens_list, t_token *token)
{
	t_token	*current;

	current = (*tokens_list);
	if (!current)
	{
		(*tokens_list) = token;
		return ;
	}
	while (current->next)
		current = current->next;
	current->next = token;
}

// t_token	*tokenize(t_minishell *minishell, char *input)
// {
// 	t_token	*tokens;
// 	int		i;
// 	int		token_count;
// 	i = 0;
// 	token_count = count_tokens(input);
// 	minishell->token_count = token_count;
// 	tokens = ft_calloc((token_count + 1), sizeof(t_token));
// 	ft_putstr_fd("There are ", 1);
// 	ft_putnbr_fd(token_count, 1);
// 	ft_putendl_fd(" tokens in your input", 1);
// 	while (i < token_count)
// 	{
// 		tokens[i].content = get_token(&input);
// 		tokens[i].type = get_token_type(tokens[i].content);
// 		if (tokens[i].type == WORD)
// 		{
// 			tokens[i].content = dollar_expansion(tokens[i].content, 			minishell->env_variables);
// 			tokens[i].content = wildcards(tokens[i].content, tokens[i].content);
// 		}
// 		i++;
// 	}
// 	tokens[i].content = NULL;
// 	return (tokens);
// }
/**
 * @brief
 * Split the input based on all the tokens
 * Sets the tokens linked list in the minishell struct to allow for convenient navigation in other functions
 *
 * @returns
 * The linked list containing all the tokens
*/
void	tokenize(t_minishell *minishell, char *input)
{
	t_token	*token;
	int		i;

	i = 0;
	minishell->token_count = count_tokens(input);
	//! For debugging purposes
	// ft_putstr_fd("There are ", 1);
	// ft_putnbr_fd(minishell->token_count, 1);
	// ft_putendl_fd(" tokens in your input", 1);
	while (i < minishell->token_count)
	{
		token = ft_calloc(1, sizeof(t_token));
		token->content = get_token(&input);
		token->type = get_token_type(token->content);
		if (token->type == WORD)
		{
			token->content = dollar_expansion(token->content, \
			minishell->env_variables);
			token->content = wildcards(token->content, token->content);
		}
		token->next = NULL;
		push_token(&minishell->tokens, token);
		i++;
	}
	minishell->tokens_head = minishell->tokens;
}
