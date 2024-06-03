/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_is_quotation.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdanish <mdanish@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 13:50:06 by maabdull          #+#    #+#             */
/*   Updated: 2024/05/28 23:50:49 by mdanish          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief Check if the provided character 'c' is a quotation mark
 * (single or double) and set the quote variable appropriately:
 * - If it already contains a quotation mark and the current character is
 *   not this, ignore it.
 * - If it contains a quotation mark and the current character is this, set
 *   it to '\0'
 *
 * @usage
 *
 * Calling this function with the input:
 * - c = '"', quote = '"'
 * 
 * Should return:
 * '\0'
 *
 * Calling this function with the input:
 * - c = '\'', quote = '"'
 * 
 * Should return:
 * '"'
 */
char	ft_is_quotation(char c)
{
	static char quote = '\0';

	if (c == '"')
	{
		if (!quote)
			quote = '"';
		else if (quote == '"')
			quote = '\0';
	}
	if (c == '\'')
	{
		if (!quote)
			quote = '\'';
		else if (quote == '\'')
			quote = '\0';
	}
	return (quote);
}
