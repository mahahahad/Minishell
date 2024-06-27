/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollar.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdanish <mdanish@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 17:04:29 by mdanish           #+#    #+#             */
/*   Updated: 2024/06/26 16:41:22 by mdanish          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Copies the pre-existing definition of a key into the token.
 * 
 * The function identifies if the length of the value is greater than the key,
 * through which it will decide whether to malloc for a new string or alter the
 * pre-existing string
 * 
 * It overwrites the [ $ ] as well as the entire key with the value, leaving
 * the rest of the token completely unaffected, regardless of the size of the
 * value.
 * 
 * @param token is the string that needs to get the value added to.
 * @param value is the string that needs to be added to the token.
 * @param start is the index at which the [ $ ] is located at.
 * @param end is the index at which the key ends.
 * 
 * @return the pointer to the final expanded token.
 */
static char	*expand_the_key(char *token, char *value, int start, int *end)
{
	char	*expanded;
	int		val_len;
	int		tok_len;

	val_len = ft_strlen(value);
	tok_len = ft_strlen(token);
	if (val_len <= *end - start)
	{
		ft_memcpy(token + start, value, val_len);
		ft_memcpy(token + start + val_len, token + *end, tok_len - *end + 1);
	}
	else
	{
		expanded = ft_calloc((tok_len + val_len) - (*end - start) + 1, 1);
		if (!expanded)
			ft_putendl_fd("Malloc failed while expanding variables.", 2);
		ft_memcpy(expanded, token, start);
		ft_memcpy(expanded + start, value, val_len);
		ft_memcpy(expanded + val_len + start, token + *end, tok_len - *end + 1);
		free(token);
		token = expanded;
	}
	*end = start + val_len;
	return (token);
}

/**
 * @brief Expands the token [ $? ] to the last status code.
 * 
 * The function mallocs for the complete expansion of the token and copies over
 * the token just before the [ $ ] is encountered. The digits are then copied to
 * the string. Then the rest of the token is copied over.
 * 
 * @param token is the string that needs to get the value added to.
 * @param start is the index at which the [ $ ] is located at.
 * @param end is the index at which the key ends.
 * @param digits is the number of digits that need to be placed into the token.
 * 
 * @return the pointer to the final expanded token.
 */
static char	*expand_status_code(char *token, int start, int end, int digits)
{
	char	*expanded;
	int		count;
	int		status;
	int		tok_len;

	status = g_status_code;
	tok_len = ft_strlen(token);
	expanded = ft_calloc(tok_len + digits - 1, sizeof(char));
	if (!expanded)
		ft_putendl_fd("Malloc failed while expanding variables.", 2);
	ft_memcpy(expanded, token, start);
	count = digits;
	while (count--)
	{
		expanded[start + count] = status % 10 + 48;
		status /= 10;
	}
	ft_memcpy(expanded + digits + start, token + end + 1, tok_len - end);
	free(token);
	return (expanded);
}

/**
 * @brief Replaces the token [ $? ] to become last status code.
 * 
 * The function identifies if the length of the value is lesser than or equal to
 * [ $? ]. If it is the case, it will alter the pre-existing string. If the
 * length of the value is greater, expand_status_code() takes care of it.
 * 
 * It overwrites the [ $ ] as well as the [ ? ] with the value, leaving the rest
 * of the token completely unaffected, regardless of the size of the value.
 * 
 * @param token is the string that needs to get the value added to.
 * @param start is the index at which the [ $ ] is located at.
 * @param end is the index at which the key ends.
 * @param length is the length of the token.
 * 
 */
static void	replace_status_code(char **token, int start, int end, int length)
{
	int		status;
	int		digits;

	status = g_status_code;
	digits = 1;
	while (status / 10 && digits++)
		status /= 10;
	if (digits == 1)
	{
		token[0][start] = g_status_code + 48;
		ft_memcpy(token[0] + end, token[0] + end + 1, length - end);
	}
	else if (digits == 2)
	{
		token[0][start] = (g_status_code / 10) + 48;
		token[0][end] = (g_status_code % 10) + 48;
	}
	else
		token[0] = expand_status_code(token[0], start, end, digits);
}

/**
 * @brief Checks for any invalid [ $ ] expansion.
 * 
 * The function identifies if there is a [ $ ] that needs to be expanded. If it
 * is found, it will check if it is the special case of [ $? ] or a variable
 * that begins with a digit. If it is the former case, it will send it to
 * replace_status_code() and if it is the latter case, ft_memcpy() is used.
 * 
 * If it is neither of the cases, it will identify the indexes at which the key
 * begins and ends, and stores those values in the parameters after
 * dereferencing them.
 * 
 * @param token is the string that is checked for a key.
 * @param start is the index at which the [ $ ] is located at.
 * @param end is the index at which the key ends.
 * @param length is the length of the token.
 * 
 * @return true if invalid key is found and false is a valid key is found.
 */
static bool	invalid_key(char **token, int *start, int *end, int length)
{
	while (token[0][(*start)++])
	{
		if (ft_is_quotation(token[0][*start - 1]) == '\'')
			continue ;
		else if (token[0][*start - 1] == '$')
			break ;
		if (!token[0][*start - 1])
			break ;
	}
	if (!token[0][*start - 1] && (*start)--)
		return (true);
	*end = *start;
	if (token[0][*end] == '?')
		return (replace_status_code(token, *start - 1, *end, length), true);
	if (ft_isdigit(token[0][*end]) && ++(*end))
		return (ft_memcpy(*token + *start - 1, *token + *end, length - *end));
	while (token[0][*end] && (ft_isalnum(token[0][*end]) || \
		token[0][*end] == '_'))
		(*end)++;
	if (*end == *start)
		return (true);
	return (false);
}

/**
 * @brief Expands any and all dollar signs if found.
 * 
 * The function uses invalid_key() to identify if the token contains any valid
 * keys. This function will also store the index at which the key starts and
 * ends by updating the variable which will be sent to it. In the case that an
 * invalid key is provided, the function will handle the behaviour for it.
 * 
 * When a valid expansion string is found, it is sent to expand_the_key(), which
 * will return the final token depending on it previous definition.
 * 
 * @param token is the token that needs to be checked for the dollar expansion.
 * @param list of the environment variables that is searched for expansions.
 * 
 * @return the pointer to the final token.
 */
char	*dollar_expansion(char *token, t_env *list)
{
	int		end;
	int		start;
	int		token_length;
	t_env	*store;

	token_length = ft_strlen(token);
	start = 0;
	while (token[start])
	{
		if (invalid_key(&token, &start, &end, token_length))
			continue ;
		store = list;
		while (store)
		{
			if (!ft_strncmp(store->key, token + start, end - start))
				break ;
			store = store->next;
		}
		if (!store && ft_memcpy(token + --start, token + end, token_length))
			continue ;
		token = expand_the_key(token, store->value, start - 1, &end);
		start = end;
	}
	return (token);
}
