/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdanish <mdanish@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 18:57:19 by mdanish           #+#    #+#             */
/*   Updated: 2024/06/10 19:00:13 by mdanish          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_lstadd_back(t_env **list, t_env *new_node)
{
	t_env	*last;

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
