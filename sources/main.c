/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdanish <mdanish@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/31 13:43:49 by maabdull          #+#    #+#             */
/*   Updated: 2024/07/21 17:04:40 by mdanish          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_status_code;

/*
 * Loops until EOF is detected and reads user input using readline
 * and executes the command in a child process and finally
 * frees the user read line.
 */
int	main(int argc, char *argv[]__attribute__((unused)), char **env)
{
	char		*line;
	t_minishell	minishell;
	t_cmd	*cmd;

	if (argc != 1)
		return (ft_putendl_fd("Minishell does not accept arguments.", 2), 1);
	g_status_code = 0;
	ft_memset(&minishell, 0, sizeof(minishell));
	setup_environment(&minishell, env);
	if (!minishell.envp)
		return (1);
	while (true)
	{
		receive_signal(PARENT);
		line = readline(B_YELLOW "minishell$ " RESET);
		if (!line)
			break ;
		cmd = parse(&minishell, line);
		if (cmd)
		{
			add_history(line);
			// run_cmd(cmd, env);
			//! FOR DEBUGGING:
			PRINT_CMD(cmd);
			free_cmd(cmd);
		}
		free_tokens(&minishell.tokens_head);
		free(line);
	}
	free_environment(&minishell);
	rl_clear_history();
	return (ft_putendl_fd("exit", 1), g_status_code);
}
