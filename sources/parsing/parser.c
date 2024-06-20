/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdanish <mdanish@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 14:41:15 by maabdull          #+#    #+#             */
/*   Updated: 2024/06/20 23:37:19 by mdanish          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
 */
char	*get_token(char **input)
{
	int		i;
	char	*token;
	char	*string;

	i = -1;
	token = NULL;
	string = *input;
	while (ft_isspace(*string))
		string++;
	while (string[++i])
	{
		if (ft_is_quotation(string[i]))
			continue;
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

t_token	*tokenize(t_minishell *minishell, char *input)
{
	t_token	*tokens;
	int		i;
	int		token_count;

	i = 0;
	token_count = count_tokens(input);
	minishell->token_count = token_count;
	tokens = malloc((token_count + 1) * sizeof(t_token));
	ft_putstr_fd("There are ", 1);
	ft_putnbr_fd(token_count, 1);
	ft_putendl_fd(" tokens in your input", 1);
	while (i < token_count)
	{
		tokens[i].content = get_token(&input);
		tokens[i].type = get_token_type(tokens[i].content);
		if (tokens[i].type == WORD)
		{
			tokens[i].content = dollar_expansion(tokens[i].content, \
			minishell->env_variables);
			tokens[i].content = wildcards(tokens[i].content, tokens[i].content);
		}
		i++;
	}
	tokens[i].content = NULL;
	return (tokens);
}

void	parse(t_minishell *minishell, char *line)
{
	if (count_quotations(line))
	{
		ft_putstr_fd(RED "Open quotes detected, command rejected.\n" RESET, 2);
		return ;
	}
	minishell->tokens = tokenize(minishell, line);
}
