/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_error.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdanish <mdanish@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 22:21:25 by maabdull          #+#    #+#             */
/*   Updated: 2024/08/05 08:37:35 by mdanish          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Error printing function.
 * 
 * This function is used whenever something unexpected happens which needs to 
 * be shared with the user.
 * The err parameter can be set to NULL to display a custom message passed in 
 * through the extra parameter.
 * 
 * @param type The type of error encountered
 * @param err
 * For syntax errors: the token near which the error was found
 * @param extra (Optional) Additonal information about the error to be printed
 */
t_cmd	*ft_print_error(t_err_type type, char *err, t_cmd *command)
{
	if (type == SYNTAX)
	{
		g_code = 2;
		ft_putstr_fd("Syntax error ", 2);
		if (err)
		{
			ft_putstr_fd("near unexpected token `", 2);
			ft_putstr_fd(err, 2);
			ft_putstr_fd("'", 2);
		}
		// if (extra)
		// 	ft_putstr_fd(extra, 2);
		ft_putendl_fd("", 2);
	}
	free_command(command);
	return (NULL);
}
