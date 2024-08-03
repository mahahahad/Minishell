/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdanish <mdanish@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/16 22:36:59 by maabdull          #+#    #+#             */
/*   Updated: 2024/08/04 00:11:23 by mdanish          ###   ########.fr       */
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
 * 
 * @return the duplicate of the token node.
 */
t_token	*token_duplicate(t_token *token)
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
 * @param type used to identify the error message that is needed to be printed.
 * @param command is sent to free_command() to free up.
 *
 * @return NULL
 */
void	*print_exec_parse_err(t_tkn_type type, t_cmd *command)
{
	g_code = 2;
	if (type == PIPE)
		ft_putendl_fd("Syntax error near unexpected token `|'", 2);
	else if (type == AND)
		ft_putendl_fd("Syntax error near unexpected token `&&'", 2);
	else if (type == OR)
		ft_putendl_fd("Syntax error near unexpected token `||'", 2);
	free_command(command);
	return (NULL);
}

/**
 * @brief Count the number of quotation pairs in the provided line.
 * 
 * @param line containes the token.
 *
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
	if (count % 2)
		g_code = 2;
	return (count % 2);
}

/**
 * @brief Count the number of bracket pairs in the provided line.
 * 
 * It goes through the line and ensures there are no open, misplaced or nested
 * brackets in the line
 * 
 * @param line is the token.
 * 
 * @return int
 */
bool	valid_brackets(char *line)
{
	int		count;

	count = 0;
	while (*line && count < 0)
	{
		if (ft_is_quotation(*line))
			line++;
		else if (*line == '(' && line++)
			count++;
		else if (*line == ')' && line++)
			count--;
		else
			line++;
	}
	if (count)
		ft_putstr_fd(RED "Bad brackets detected, command rejected.\n" RESET, 2);
	if (count)
		g_code = 2;
	return (!count);
}

/**
 * @brief Count how many tokens there are in the provided input string.
 * 
 * Uses spaces and special chars as delimiters but ignores them in quoted
 * strings.
 * 
 * @param input is the prompt.
 * 
 * @return the number of tokens present inside of the prompt.
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
			if (!ft_strchr("()", input[i]) && input[i] == input[i + 1])
				i++;
			return (count_tokens(input + i + 1) + 1 + extra_token);
		}
	}
	return (i != 0);
}
