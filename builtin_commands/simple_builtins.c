/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simple_builtins.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdanish <mdanish@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 23:42:04 by mdanish           #+#    #+#             */
/*   Updated: 2024/04/04 00:55:00 by mdanish          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_exit(int status)				// Ideally should recieve the struct.
{
	// Implement all the closes and the frees.
	exit(status);
}

void	ft_echo(char *line, bool flag)	// Ideally should recieve the struct.
{
	if (line)
		printf("%s", line);
	if (!flag)
		printf("\n");
	/*
		The parsing struct will contain the line as well as the flag.
		The struct will also contain a variable for the last status code.
		That status code will be updated to 0, 
		and the struct will be sent to the function ft_exit.
	*/
	ft_exit(0);
}

void	ft_env(t_env *env)				// Ideally should recieve the struct.
{
	while (env)
	{
		printf("%s", env->env_var);
		env = env->next;
	}
	/*
		The parsing struct will contain the environment variables.
		The struct will also contain a variable for the last status code.
		That status code will be updated to 0, 
		and the struct will be sent to the function ft_exit.
	*/
	ft_exit(0);
}
