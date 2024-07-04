/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maabdull <maabdull@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 22:33:34 by maabdull          #+#    #+#             */
/*   Updated: 2024/07/04 22:37:21 by maabdull         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_signal_parent(int signum)
{
	if (signum == SIGINT)
	{
		ft_putchar_fd('\n', 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		g_status_code = 130;
	}
}

void	handle_signal_child(int signum)
{
	int	status;

	waitpid(-1, &status, 0);
	if (WIFEXITED(status) == false)
	{
		if (signum == SIGINT)
		{
			ft_putendl_fd("", 1);
			g_status_code = 130;
		}
		else if (signum == SIGQUIT)
		{
			ft_putendl_fd("Quit: 3", 1);
			g_status_code = 131;
		}
	}
}

/**
 * @brief Main signal handling function.
 * Checks if the CHILD or PARENT process should have their signals handled
 * using the parameter passed to the function.
 * 
 * @param receiver 
 */
void	receive_signal(t_signal_receiver receiver)
{
	if (receiver == PARENT)
	{
		signal(SIGINT, handle_signal_parent);
		signal(SIGQUIT, SIG_IGN);
	}
	if (receiver == CHILD)
	{
		signal(SIGINT, handle_signal_child);
		signal(SIGQUIT, handle_signal_child);
	}
}
