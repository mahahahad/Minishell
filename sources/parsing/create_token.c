/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_token.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdanish <mdanish@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 17:12:14 by mdanish           #+#    #+#             */
/*   Updated: 2024/08/04 00:08:22 by mdanish          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
 * @param input is the prompt from the readline
 *
 * @return a single token from the prompt.
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
		if (ft_strchr("|<>()&", (*input)[i]))
		{
			if ((*input)[i] == (*input)[i + 1] && !i)
				i = 2;
			if (!i)
				i = 1;
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
 * @param content is the token
 * 
 * @return the type of token that content is.
 */
static t_tkn_type	get_token_type(char *content)
{
	if (content[0] && !content[1])
	{
		if (content[0] == '|')
			return (PIPE);
		if (content[0] == '>')
			return (GREAT);
		if (content[0] == '<')
			return (LESS);
		if (content[0] == '(')
			return (P_OPEN);
		if (content[0] == ')')
			return (P_CLOSE);
	}
	else if (content[0] && content[1] && !content[2])
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
 * @brief Removes the quotes from the token.
 * 
 * For all the tokens of type [ WORD ], this function identifies any quotes in
 * the token at removes it. This is done by using two indexes and copying the
 * indexs over from within the string itself, all while ignoring the quotes that
 * are needed to be ommited.
 * 
 * @param token is the string that contains the token itself.
 * 
 * @return The token after the quotes are removed.
 */
static char	*quote_trimming(char *token)
{
	int		token_index;
	int		quote_index;
	char	quote;

	token_index = 0;
	quote_index = 0;
	quote = '\0';
	while (token[quote_index])
	{
		if ((token[quote_index] == '"' || token[quote_index] == '\'') && !quote)
			quote = token[quote_index++];
		else if (quote == token[quote_index] && ++quote_index)
			quote = '\0';
		else
			token[token_index++] = token[quote_index++];
	}
	token[token_index] = token[quote_index];
	return (token);
}

/**
 * @brief Creates a new node for the token list.
 * 
 * The function mallocs for t_token node and stores within it the content that
 * is provided as an argument. It then calls get_token_type() to identify the
 * type of the content and is stored within the token. For tokens of the type
 * [ WORD ], dollar_expansion() and wildcard_expansion() are called to take care
 * of all the expansion part of the parsing.
 * 
 * @param content is the content of the token.
 * @param list refers to the environment variables.
 * 
 * @return the token created and NULL in case of error.
 */
t_token	*new_token(char *content, t_env *list, bool expand)
{
	t_token	*token;
	t_token	*store;

	if (!content)
		return (NULL);
	token = ft_calloc(1, sizeof(t_token));
	if (!token)
		return (free(content), g_code = 1, NULL);
	token->content = content;
	token->type = get_token_type(token->content);
	if (token->type == WORD && expand)
	{
		token->content = dollar_expansion(token->content, list);
		if (!token->content)
			return (NULL);
		store = wildcard_expansion(token->content);
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

/**
 * @brief Add the specified token to the end of the tokens list
 * 
 * @param tokens_list is the pointer to the list of tokens.
 * @param token is the new token that is to be added to the tokens_list.
 */
void	add_token_back(t_token **tokens_list, t_token *token)
{
	t_token	*current;

	if (!token)
		return (perror("Tokenisation"), g_code = 1, \
			free_tokens(tokens_list));
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
