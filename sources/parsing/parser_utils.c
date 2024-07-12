/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdanish <mdanish@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/16 22:36:59 by maabdull          #+#    #+#             */
/*   Updated: 2024/07/07 19:20:53 by mdanish          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * This file contains the utility functions used for the main parsing functions
 */

/**
 * @brief Allocate space for, and copy, only the first node of the provided
 * token node.
 * Useful for copying tokens into a command struct without it's
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
 * @brief Print the appropriate error message indicating a symbol was found
 * in a location it should not have been in.
 *
 * @param minishell
 * @return NULL
 */
void	*print_exec_parse_err(t_minishell *minishell)
{
	g_status_code = 2;
	if (minishell->tokens->type == PIPE)
		ft_putendl_fd("Syntax error near unexpected token `|'", 2);
	else if (minishell->tokens->type == AND)
		ft_putendl_fd("Syntax error near unexpected token `&&'", 2);
	else if (minishell->tokens->type == OR)
		ft_putendl_fd("Syntax error near unexpected token `||'", 2);
	return (NULL);
}

/**
 * @brief Is the current token's type one that should break the executable
 * command loop (PIPE, AND, OR)
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
 * @brief Count the number of quotation pairs in the provided line.
 * 
 * @param char*
 * @return int
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
 * @brief Count the number of bracket pairs in the provided line.
 * 
 * It goes through the line and ensures there are no open, misplaced or nested
 * brackets in the line
 * 
 * @param char*
 * @return int
 */
bool	valid_brackets(char *line)
{
	int		count;
	int		index;

	index = -1;
	count = 0;
	while (line[++index] && count > -1 && count < 2)
	{
		if (ft_is_quotation(line[index]))
			continue ;
		if (line[index] == '(')
			count++;
		else if (line[index] == ')')
			count--;
	}
	if (count)
		ft_putstr_fd(RED "Bad brackets detected, command rejected.\n" RESET, 2);
	return (!count);
}

/**
 * @brief Check if the provided character is in the list of special characters:
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

/** @brief Check if a character is in the list of repeatable characters:
 * - [<<]
 * - [>>]
 * - [||]
 * - [&&]
 */
bool	is_repeatable_char(char c1, char c2)
{
	if ((c1 == '<' && c2 == '<') \
			|| (c1 == '>' && c2 == '>') \
			|| (c1 == '|' && c2 == '|') \
			|| (c1 == '&' && c2 == '&'))
		return (true);
	return (false);
}

/**
 * @brief Return the next token in the provided input string.

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
 * @param char**
 * @return char*
 */
char	*get_token(char **input)
{
	int		i;

	i = -1;
	while (ft_isspace(**input))
		(*input)++;
	while ((*input)[++i])
	{
		if (ft_is_quotation((*input)[i]))
			continue ;
		if (is_delimiter((*input)[i]))
		{
			if (is_repeatable_char((*input)[i], (*input)[i + 1]) && !i)
				i += 2;
			if (!i)
				i++;
			break ;
		}
		if (ft_isspace((*input)[i]) && (i || ++i))
			break ;
	}
	*input += i;
	return (ft_substr(*input - i, 0, i));
}

/**
 * @brief Identify the type of the token passed using the content parameter
 *
 * Returns word by default if the content parameter doesn't satisfy any of the 
 * requirements
 * 
 * @param char*
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
 * @brief Count how many tokens there are in the provided input string.
 * 
 * Uses spaces and special chars as delimiters but ignores them in quoted
 * strings.
 * 
 * @param char*
 * @return int
 */
int	count_tokens(char *input)
{
	int		i;
	bool	extra_token;

	i = -1;
	while (ft_isspace(*input))
		input++;
	while (input[++i])
	{
		if (ft_is_quotation(input[i]))
			continue ;
		if (ft_isspace(input[i]) && input[i + 1])
			return (count_tokens(input + i + 1) + 1);
		if (is_delimiter(input[i]))
		{
			extra_token = false;
			if (i > 0 && !ft_isspace(input[i - 1]))
				extra_token = true;
			if (is_repeatable_char(input[i], input[i + 1]))
				i++;
			return (count_tokens(input + i + 1) + 1 + extra_token);
		}
	}
	return (i != 0);
}

/**
 * @brief Frees up the token list.
 * 
 * Simply goes through the list and frees the content and the node itself.
 * It can be used in case of malloc fail or when performing a general cleanup.
 * 
 * @param list is the list of tokens.
 */
void	clear_tokens(t_token **list)
{
	t_token	*current;
	t_token	*next;

	current = *list;
	while (current)
	{
		next = current->next;
		free(current->content);
		free(current);
		current = next;
	}
	*list = NULL;
}

/**
 * @brief Add the specified token to the end of the tokens list
 * 
 * @param t_token**
 * @param t_token*
 */
void	add_token_back(t_token **tokens_list, t_token *token)
{
	t_token	*current;

	if (!token)
	{
		ft_putendl_fd("Malloc while tokenising the prompt failed.", 2);
		clear_tokens(tokens_list);
		return ;
	}
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

/**
 * @brief Creates a new node for the token list.
 * 
 * The function mallocs for t_token node and stores within it the content that
 * is provided as an argument. It then calls get_token_type() to identify the
 * type of the content and is stored within the token. For tokens of the type
 * [ WORD ], dollar_expansion() and wildcards() are called to take care of all
 * the expansion part of the parsing.
 * 
 * @param content is the content of the token.
 * @param list refers to the environment variables.
 * 
 * @return the token created and NULL in case of error.
 */
t_token	*new_token(char *content, t_env *list)
{
	t_token	*token;
	t_token	*store;

	if (!content)
		return (NULL);
	token = ft_calloc(1, sizeof(t_token));
	if (!token)
		return (NULL);
	token->content = content;
	token->type = get_token_type(token->content);
	if (token->type == WORD && list)
	{
		token->content = dollar_expansion(token->content, list);
		store = wildcards(token->content);
		if (store)
		{
			free(content);
			free(token);
			return (store);
		}
		token->content = quote_trimming(token->content);
	}
	return (token);
}
