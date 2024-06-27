/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maabdull <maabdull@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/31 13:43:49 by maabdull          #+#    #+#             */
/*   Updated: 2024/06/26 15:02:17 by maabdull         ###   ########.fr       */
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
 * NOTE: Readline causes still reachable memory leaks
 */
int	main(int argc, char *argv[] __attribute__((unused)), char **env)
{
	char		*line;
	t_minishell	minishell;
	// t_cmd	*cmd;

	if (argc != 1)
		return (ft_putendl_fd("Minishell can not run external files.", 2), 1);
	g_status_code = 0;
	signal(SIGINT, handle_sigint);
	ft_memset(&minishell, 0, sizeof(minishell));
	setup_environment(&minishell, env);
	minishell.prompt = init_prompt();
	line = readline(minishell.prompt->current);
	while (line)
	{
		t_cmd *cmd = parse(&minishell, line);
		if (cmd)
			add_history(line);
		// cmd = create_exec_cmd(&minishell);
		run_cmd(cmd, env);
		// free_cmd(cmd);
		free(line);
		// if (minishell.token_count)
		// {
		// 	for (int i = 0; i < minishell.token_count; i++){
		// 			print_token(minishell.tokens[i]);
		// 	}
		// 	free_tokens(&minishell);
		// }
		update_prompt(minishell.prompt);
		line = readline(minishell.prompt->current);
	}
	free(minishell.prompt->previous);
	free(minishell.prompt->current);
	free(minishell.prompt);
	ft_putendl_fd("exit", 1);
	return (g_status_code);
}
