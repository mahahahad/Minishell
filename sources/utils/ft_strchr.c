/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maabdull <maabdull@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/28 20:24:10 by maabdull          #+#    #+#             */
/*   Updated: 2024/05/02 22:38:56 by maabdull         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/* 
 * @brief Locates the first occurence of 'c' in the string 's'
 * @param s The string to search through
 * @param c The character to find
 * @return The remaining part of the string with 'c' as the first character
 */
char	*ft_strchr(const char *s, int c)
{
	int	i;

	i = -1;
	while (s[++i])
		if (s[i] == (char) c)
			return ((char *) s + i);
	if (s[i] == c)
		return ((char *) s + i);
	return (NULL);
}
