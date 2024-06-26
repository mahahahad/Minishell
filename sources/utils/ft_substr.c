/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdanish <mdanish@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 16:00:35 by maabdull          #+#    #+#             */
/*   Updated: 2024/06/26 16:44:19 by mdanish          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/// @brief
	// Creates a substring from the string 's' starting from 'start' with the
	// length 'len'
/// @param s
	// The string to create a substring from
/// @param start
	// The index from which to start creating the substring
/// @param len
	// The number of characters to include in the substring
/// @return
	// The newly created substring
char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	i;
	char	*substr;

	i = 0;
	if (!s)
		return (NULL);
	if ((size_t) start >= ft_strlen(s))
		len = 0;
	if (len > ft_strlen(s + start))
		len = ft_strlen(s + start);
	substr = ft_calloc((len + 1), sizeof(char));
	if (!substr)
		return (NULL);
	while (i != len)
		substr[i++] = s[start++];
	substr[len] = '\0';
	return (substr);
}
