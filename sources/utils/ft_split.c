/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdanish <mdanish@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/17 13:14:03 by maabdull          #+#    #+#             */
/*   Updated: 2024/06/26 16:43:35 by mdanish          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	count_words(const char *s, const char c)
{
	int	words;
	int	word_found;
	int	i;

	words = 0;
	word_found = 0;
	i = -1;
	while (s[++i])
	{
		if (s[i] != c && !word_found)
		{
			word_found = 1;
			words++;
		}
		if (s[i] == c)
			word_found = 0;
	}
	return (words);
}

/// @brief
// Splits a string 's' by using 'c' as a delimiter
// All instances of the char 'c' are ignored
/// @param s
// The string to split
/// @param c
// The character to split based on
/// @return
// An array of the newly obtained strings
char	**ft_split(char const *s, char c)
{
	int		word_start_index;
	size_t	i;
	int		j;
	char	**strings;

	if (!s)
		return (NULL);
	strings = ft_calloc((count_words(s, c) + 1), sizeof(char *));
	if (!strings)
		return (NULL);
	word_start_index = -1;
	i = -1;
	j = 0;
	while (++i != ft_strlen(s) + 1)
	{
		if ((s[i] == c || i == ft_strlen(s)) && word_start_index >= 0)
		{
			strings[j++] = ft_substr(s, word_start_index, i - word_start_index);
			word_start_index = -1;
		}
		if (s[i] != c && word_start_index < 0)
			word_start_index = i;
	}
	strings[j] = NULL;
	return (strings);
}
