/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdanish <mdanish@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/15 19:48:44 by mdanish           #+#    #+#             */
/*   Updated: 2024/07/15 12:26:20 by mdanish          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	word_length(char const *string, char delimiter)
{
	size_t	length;

	length = 0;
	while (*string && *string++ != delimiter)
		length++;
	return (length);
}

size_t	word_counter(char const *string, char delimiter)
{
	size_t	ctr;

	ctr = 0;
	while (*string)
	{
		while (*string == delimiter && *string)
			string++;
		if (*string)
			ctr++;
		while (*string != delimiter && *string)
			string++;
	}
	return (ctr);
}

void	free_split(char **split, size_t word_count)
{
	while (split && word_count--)
		free(*(split + word_count));
	free(split);
}

char	**ft_split(char const *string, char delimiter)
{
	char	**split;
	char	**result;
	size_t	word_count;
	size_t	word_len;

	word_count = word_counter(string, delimiter);
	result = malloc((word_count + 1) * sizeof(char *));
	if (!result)
		return (NULL);
	split = result;
	while (word_count--)
	{
		while (*string == delimiter && *string)
			string++;
		word_len = word_length(string, delimiter);
		*result = ft_substr(string, 0, word_len);
		if (!(*result))
			return (free_split(split, result - split), NULL);
		string += word_len;
		result++;
	}
	*result = NULL;
	return (split);
}
