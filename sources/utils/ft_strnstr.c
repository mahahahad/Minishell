/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdanish <mdanish@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 17:56:24 by mdanish           #+#    #+#             */
/*   Updated: 2024/08/07 18:26:42 by mdanish          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

bool	ft_strnstr(char *haystack, char *needle, int length)
{
	int	index;
	int	store;

	index = 0;
	while (*haystack)
	{
		store = length;
		while (store && haystack[index] == needle[index] && ++index)
			store--;
		if (!store)
			return (true);
		haystack++;
	}
	return (false);
}
