/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maabdull <maabdull@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/31 13:43:49 by maabdull          #+#    #+#             */
/*   Updated: 2024/07/30 19:11:37 by maabdull         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_code;

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
	while (true)
	{
		receive_signal(PARENT);
		line = readline(B_YELLOW "minishell$ " RESET);
		if (!line)
			break ;
		minishell.input_fd = false;
		minishell.output_fd = false;
		minishell.cmd = NULL;
		parse(&minishell, line, line);
		// run_cmd(&minishell, -1);
		//! FOR DEBUGGING:
		PRINT_CMD(minishell.cmd);
		free_parsing(&minishell);
	}
	free_environment(&minishell);
	rl_clear_history();
	return (ft_putendl_fd("exit", 1), g_code);
}
