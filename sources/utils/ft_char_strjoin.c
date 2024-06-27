/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_char_strjoin.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdanish <mdanish@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 12:11:34 by maabdull          #+#    #+#             */
/*   Updated: 2024/06/26 16:43:11 by mdanish          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief Joins the provided strings with the provided character as a separator
 */
char	*ft_char_strjoin(char const *s1, char const *s2, char const c)
{
	char	*final_str;
	int		i;
	int		j;

	i = -1;
	j = 0;
	if (!s1 || !s2)
		return (NULL);
	final_str = ft_calloc(ft_strlen(s1) + ft_strlen(s2) + 2, sizeof(char));
	if (!final_str)
		return (NULL);
	while (s1[++i])
		final_str[i] = s1[i];
	final_str[i++] = c;
	while (s2[j])
		final_str[i++] = s2[j++];
	final_str[i] = '\0';
	return (final_str);
}
