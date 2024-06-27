/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdanish <mdanish@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 16:00:08 by maabdull          #+#    #+#             */
/*   Updated: 2024/06/26 16:44:07 by mdanish          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/// @brief
	// Joins the strings 's1' and 's2' and NULL terminates
/// @param s1
	// The first string to join
/// @param s2
	// The second string to join
/// @return
	// A new string that is a combination of 's1' and 's2'
char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*final_str;
	int		i;
	int		j;

	i = -1;
	j = 0;
	if (!s1 || !s2)
		return (NULL);
	final_str = ft_calloc(ft_strlen(s1) + ft_strlen(s2) + 1, sizeof(char));
	if (!final_str)
		return (NULL);
	while (s1[++i])
		final_str[i] = s1[i];
	while (s2[j])
		final_str[i++] = s2[j++];
	final_str[i] = '\0';
	return (final_str);
}
