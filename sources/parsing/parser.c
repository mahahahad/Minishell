/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maabdull <maabdull@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 14:41:15 by maabdull          #+#    #+#             */
/*   Updated: 2024/05/04 14:43:00 by maabdull         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_quotations(char *line)
{
	int	count;
	int	i;
	char	quotes_found;

	i = -1;
	count = 0;
	quotes_found = '\0';
	while (line[++i])
	{
		if (line[i] == '"')
		{
			if (!quotes_found)
			{
				quotes_found = '"';
				count++;
			}
			else if (quotes_found == '"')
			{
				quotes_found = '\0';
				count++;
			}
			continue ;
		}
		if (line[i] == '\'')
		{
			if (!quotes_found)
			{
				quotes_found = '\'';
				count++;
			}
			else if (quotes_found == '\'')
			{
				quotes_found = '\0';
				count++;
			}
			continue ;
		}
	}
	return (count);
}

/**
 * @brief Skip the leading whitespace characters by moving the pointer to the
 * head of the string ahead.
 * Move the pointer to the head of the string to the start of the next token
 * or '\0' if there isn't one and return this token.
 *
 * @usage
 *
 * Calling the function with the input:
 * - '       This token '
 *
 * will result in the function returning "This" and input pointing to 't' in
 * 'token'.
 *
 * Calling the function with the input:
 * - 'token'
 *
 * will result in the function returning "token" and input pointing to '\0'.
 */
char	*get_token(char	**input)
{
	int	i;
	char	*token;
	char	*string;
	char	quotes_found;
	bool	space_found;

	i = 0;
	quotes_found = '\0';
	space_found = false;
	token = NULL;
	string = *input;
	while (ft_isspace(*string))
		string++;
	while (string[i])
	{
		quotes_found = ft_is_quotation(string[i], quotes_found);
		if (!quotes_found && !space_found && i > 0 && ft_isspace(string[i]))
		{
			space_found = true;
			break ;
		}
		if (space_found && !quotes_found && !ft_isspace(string[i]))	
			space_found = false;
		i++;
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
	}
	return (WORD);
}

/**
 * @brief Counts how many tokens there are in the provided input string
 * Uses spaces as a delimiter but ignores them in quoted strings
 */
int	count_tokens(char *input)
{
	int		i;
	int		j;
	bool	space_found;
	char	quotes_found;

	i = 0;
	j = 0;
	quotes_found = '\0';
	space_found = false;
	while (*input == ' ')
		input++;
	while (input[i])
	{
		if (input[i] == '"')
		{
			if (!quotes_found)
				quotes_found = '"';
			else if (quotes_found == '"')
				quotes_found = '\0';
		}
		if (input[i] == '\'')
		{
			if (!quotes_found)
				quotes_found = '\'';
			else if (quotes_found == '\'')
				quotes_found = '\0';
		}
		if (!quotes_found && !space_found && input[i] == ' ')
		{
			if (input[i + 1] && input[i + 1] != ' ')
			{
				j++;
				space_found = true;
			}
		}
		if (!quotes_found && input[i] != ' ')	
			space_found = false;
		i++;
	}
	if (i > 0)
		j++;
	return (j);
}

t_token	*tokenize(t_minishell *minishell, char *input)
{
	t_token	*tokens;
	int	i;
	int	token_count;

	i = 0;
	token_count = count_tokens(input);
	minishell->token_count = token_count;
	tokens = malloc((token_count + 1) * sizeof(t_token));
	while (i < token_count)
	{
		tokens[i].content = get_token(&input);
		tokens[i].type = get_token_type(tokens[i].content);
		i++;
	}
	tokens[i].content = NULL;
	return (tokens);
}

void	parse(t_minishell *minishell, char *line)
{
	if (count_quotations(line) % 2 != 0)
		fprintf(stderr, RED "Open quotes detected, command rejected.\n" RESET);
	minishell->tokens = tokenize(minishell, line);
}
