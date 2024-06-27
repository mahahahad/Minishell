/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maabdull <maabdull@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 12:31:23 by maabdull          #+#    #+#             */
/*   Updated: 2024/06/24 12:32:26 by maabdull         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * This file contains general debugging functions such as printing tokens.
 */

#include "minishell.h"

void	print_token(t_token token)
{
	char	*type;

	switch (token.type){
		case PIPE:
			type = "|";
			break ;
		case LESS:
			type = "<";
			break ;
		case DBL_LESS:
			type = "<<";
			break ;
		case GREAT:
			type = ">";
			break ;
		case DBL_GREAT:
			type = ">>";
			break ;
		case OR:
			type = "OR";
			break ;
		case AND:
			type = "AND";
			break ;
		default :
			type = "Word";
	}
	ft_putstr_fd("{\n\tContent: ", 1);
	ft_putstr_fd(token.content, 1);
	ft_putstr_fd("\n\tType: ", 1);
	ft_putstr_fd(type, 1);
	ft_putstr_fd("\n}\n", 1);
}
