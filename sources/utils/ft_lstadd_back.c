/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdanish <mdanish@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 18:57:19 by mdanish           #+#    #+#             */
/*   Updated: 2024/05/13 18:57:37 by mdanish          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_lstadd_back(t_env_node **list, t_env_node *new_node)
{
	t_env_node *last;

	if (!*list)
		*list = new_node;
	else
	{
		last = *list;
		while (last->next)
			last = last->next;
		last->next = new_node;
	}
}