/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdanish <mdanish@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/03 20:02:20 by mdanish           #+#    #+#             */
/*   Updated: 2024/07/21 17:33:19 by mdanish          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *destination, void *source, size_t bytes)
{
	void	*dest;

	dest = destination;
	if (!source || !destination)
		return (NULL);
	while (bytes--)
		*(char *)destination++ = *(char *)source++;
	return (dest);
}
