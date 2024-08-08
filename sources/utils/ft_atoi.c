/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdanish <mdanish@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 16:19:26 by mdanish           #+#    #+#             */
/*   Updated: 2024/08/07 06:37:59 by mdanish          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

long	ft_atoi(char const *string)
{
	int			sign;
	long int	number;

	number = 0;
	sign = 1;
	while (*string == 32 || (*string > 8 && *string < 14))
		string++;
	if (*string == '+')
		string++;
	else if (*string == '-' && string++)
		sign = -1;
	while (*string == 48)
		string++;
	while (*string > 47 && *string < 58)
		number = (number * 10) + *string++ - 48;
	return (number * sign);
}
