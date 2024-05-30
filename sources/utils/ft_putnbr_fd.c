/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdanish <mdanish@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/16 15:21:30 by mdanish           #+#    #+#             */
/*   Updated: 2024/05/29 12:14:29 by mdanish          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putnbr_fd(long int number, int fd)
{
	if (fd >= 0)
	{
		if (number > -1)
			return (ft_putunbr_base_fd(number, fd, DECIMAL, 10));
		ft_putchar_fd('-', fd);
		return (ft_putunbr_base_fd(number * -1, fd, DECIMAL, 10));
	}
}
