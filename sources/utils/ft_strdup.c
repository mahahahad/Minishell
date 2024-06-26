/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdanish <mdanish@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 15:59:57 by maabdull          #+#    #+#             */
/*   Updated: 2024/06/26 16:43:52 by mdanish          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/// @brief
	// Duplicates the string 'src' by allocating space for it into another
	// string
/// @param src
	// The string to duplicate
/// @return
	// The duplicated string
char	*ft_strdup(const char *src)
{
	char	*dup;
	size_t	src_len;

	src_len = ft_strlen(src) + 1;
	dup = ft_calloc(src_len, sizeof(char));
	if (!dup)
		return (NULL);
	ft_strlcpy(dup, src, src_len);
	return (dup);
}
