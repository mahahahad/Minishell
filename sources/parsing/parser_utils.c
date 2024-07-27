/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maabdull <maabdull@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/16 22:36:59 by maabdull          #+#    #+#             */
/*   Updated: 2024/07/22 18:53:39 by maabdull         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * This file contains the utility functions used for the main parsing functions
 */

/**
 * @brief Duplicate the node provided as the argument.
 *
 * @param token is the node that needs to be duplicated.
 * @return the duplicate of the token node.
 */
t_token	*tokendup(t_token *token)
{
	t_token	*token_copy;

	token_copy = ft_calloc(1, sizeof(t_token));
	if (!token_copy)
		return (NULL);
	token_copy->content = ft_strdup(token->content);
	if (!token_copy->content)
		return (free(token_copy), NULL);
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
void	*print_exec_parse_err(t_tkn_type type, t_cmd *cmd)
{
	g_status_code = 2;
	if (type == PIPE)
		ft_putendl_fd("Syntax error near unexpected token `|'", 2);
	else if (type == AND)
		ft_putendl_fd("Syntax error near unexpected token `&&'", 2);
	else if (type == OR)
		ft_putendl_fd("Syntax error near unexpected token `||'", 2);
	free_cmd(cmd);
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
bool	is_exec_delimiter(t_tkn_type type)
{
	return (type == PIPE || type == AND || type == OR || type == PARAN_CLOSE);
}

/**
 * @brief Count the number of quotation pairs in the provided line.
 * 
 * @param char*
 * @return true if valid quotes and false if invalid quotes.
 */
bool	count_quotations(char *line)
{
	int		count;
	int		index;
	char	quotes_found;

	index = -1;
	count = 0;
	quotes_found = '\0';
	while (line[++index])
	{
		if (line[index] == '"' || line[index] == '\'')
		{
			if (!quotes_found && ++count)
				quotes_found = line[index];
			else if (quotes_found == line[index] && ++count)
				quotes_found = '\0';
		}
	}
	if (count % 2)
		ft_putstr_fd(RED "Open quotes detected, command rejected.\n" RESET, 2);
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
	while (line[++index] && (!count || count == 1))
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
		if (content[0] == '(')
			return (PARAN_OPEN);
		if (content[0] == ')')
			return (PARAN_CLOSE);
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
		if (ft_strchr("|<>()&", input[i]))
		{
			extra_token = false;
			if (i > 0 && !ft_isspace(input[i - 1]))
				extra_token = true;
			if (input[i] == input[i + 1])
				i++;
			return (count_tokens(input + i + 1) + 1 + extra_token);
		}
	}
	return (i != 0);
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
		return (ft_putendl_fd("Malloc failed during tokenisation.", 2), \
			g_status_code = 1, free_tokens(tokens_list));
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
char	*quote_trimming(char *token)
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
		return (free(content), g_status_code = 1, NULL);
	token->content = content;
	token->type = get_token_type(token->content);
	if (token->type == WORD && list)
	{
		token->content = dollar_expansion(token->content, list);
		if (!token->content)
			return (NULL);
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
