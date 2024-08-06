/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdanish <mdanish@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/31 13:43:49 by maabdull          #+#    #+#             */
/*   Updated: 2024/08/06 16:12:55 by mdanish          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

unsigned int	g_code;

/*
 * Loops until EOF is detected and reads user input using readline
 * and executes the command in a child process and finally
 * frees the user read line.
 */
int	main(int argc, char *argv[]__attribute__((unused)), char **env)
{
	char		*line;
	t_minishell	minishell;

	if (argc != 1)
		return (ft_putendl_fd("Minishell does not accept arguments.", 2), 1);
	g_code = 0;
	setup_environment(&minishell, env);
	receive_signal(PARENT);
	while (true)
	{
		line = readline(B_YELLOW "minishell$ " RESET);
		if (!line)
			break ;
		minishell.input_fd = false;
		minishell.output_fd = false;
		minishell.pipe_read_store = -1;
		parse(&minishell, line, line);
		run_command(&minishell, false);
		//! FOR DEBUGGING:
		// PRINT_CMD(minishell.cmd_head);
		free_parsing(&minishell);
	}
	return (rl_clear_history(), free_environment(&minishell),
		ft_putendl_fd("exit", 1), g_code);
}
