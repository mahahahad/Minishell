/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin_free.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdanish <mdanish@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 17:06:47 by mdanish           #+#    #+#             */
/*   Updated: 2024/08/07 19:02:05 by mdanish          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strjoin_free(char *s1, char *s2, int free_string)
{
	char	*final_string;

	if (!s1 && !s2)
		return (NULL);
	if (!s1)
		final_string = ft_strjoin("", s2);
	if (!s2)
		final_string = ft_strjoin(s1, "");
	if (s1 && s2)
		final_string = ft_strjoin(s1, s2);
	if (free_string == 1 || free_string == 3)
		free(s1);
	if (free_string == 2 || free_string == 3)
		free(s2);
	return (final_string);
}

char	*ft_c_strjoin_free(char *s1, char *s2, char c, int free_string)
{
	char	*final_string;

	if (!s1 && !s2)
		return (ft_strdup(&c));
	if (!s1)
		final_string = ft_strdup(s2);
	else if (!s2)
		final_string = ft_strdup(s1);
	else
		final_string = ft_char_strjoin(s1, s2, c);
	if (free_string == 1 || free_string == 3)
		free(s1);
	if (free_string == 2 || free_string == 3)
		free(s2);
	return (final_string);
}
