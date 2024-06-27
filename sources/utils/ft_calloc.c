/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdanish <mdanish@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/05 08:28:07 by mdanish           #+#    #+#             */
/*   Updated: 2024/06/26 16:35:11 by mdanish          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t count, size_t size)
{
	void	*pointer;

	if (size && (count > (UINT32_MAX / size)))
		return (NULL);
	pointer = (void *)malloc(count * size);
	if (!pointer)
		return (NULL);
	ft_memset(pointer, 0, (count * size));
	return (pointer);
}
