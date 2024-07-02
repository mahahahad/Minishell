/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maabdull <maabdull@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/31 13:43:49 by maabdull          #+#    #+#             */
/*   Updated: 2024/07/02 22:15:55 by maabdull         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_status_code;

void	handle_sigint(int signum)
{
	(void)signum;
	ft_putchar_fd('\n', 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	g_status_code = 130;
}

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
	signal(SIGINT, handle_sigint);
	ft_memset(&minishell, 0, sizeof(minishell));
	setup_environment(&minishell, env);
	while (true)
	{
		line = readline(B_YELLOW "minishell$ " RESET);
		if (!line)
			break ;
		cmd = parse(&minishell, line);
		if (cmd)
			add_history(line);
		run_cmd(cmd, env);
		// free_cmd(cmd);
		free(line);
		//! for debugging, to be removed
		// minishell.tokens = minishell.tokens_head;
		// if (minishell.token_count)
		// {
		// 	for (int i = 0; i < minishell.token_count; i++){
		// 			print_token((*minishell.tokens));
		// 			minishell.tokens = minishell.tokens->next;
		// 	}
		// free_tokens(&minishell);
		// }
	}
	ft_putendl_fd("exit", 1);
	return (g_status_code);
}
