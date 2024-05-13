/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdanish <mdanish@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/03 16:44:37 by mdanish           #+#    #+#             */
/*   Updated: 2024/05/13 16:46:33 by mdanish          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *string, int character, size_t length)
{
	unsigned char	*str;

	if (!string)
		return (NULL);
	str = (unsigned char *)string;
	while (length--)
		*str++ = character;
	return (string);
}
