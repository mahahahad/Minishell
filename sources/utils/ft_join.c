/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_join.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maabdull <maabdull@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/28 16:45:53 by maabdull          #+#    #+#             */
/*   Updated: 2024/04/28 16:50:35 by maabdull         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_join(char **tab)
{
	char	*string;
	char	*temp;
	int		i;

	i = 0;
	string = tab[0];
	while (tab[i + 1])
	{
		temp = string;
		string = ft_strjoin(string, tab[i + 1]);
		free(temp);
		i++;
	}
	return (string);
}
