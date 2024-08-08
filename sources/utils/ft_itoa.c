/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdanish <mdanish@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/16 12:21:42 by mdanish           #+#    #+#             */
/*   Updated: 2024/08/07 06:48:25 by mdanish          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	digit_counter(long number)
{
	int	counter;

	if (number == 0)
		return (1);
	counter = 0;
	if (number < 0)
		counter++;
	while (number)
	{
		counter++;
		number /= 10;
	}
	return (counter);
}

char	*ft_itoa(long num)
{
	int		counter;
	size_t	number;
	char	*string;

	counter = digit_counter(num);
	string = (char *)malloc(sizeof(char) * (counter + 1));
	if (!string)
		return (NULL);
	*(string + counter) = '\0';
	if (num < 0)
		number = num * -1;
	else
		number = num;
	while (counter--)
	{
		*(string + counter) = (number % 10 + 48);
		number /= 10;
	}
	if (num < 0)
		*string = '-';
	return (string);
}
