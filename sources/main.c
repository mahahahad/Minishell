/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maabdull <maabdull@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/31 13:43:49 by maabdull          #+#    #+#             */
/*   Updated: 2024/05/05 12:46:19 by maabdull         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_status_code;

// void	handle_sigint(int signum)
// {
// 	(void)signum;
// 	puts("");
// 	rl_on_new_line();
// 	rl_replace_line("", 0);
// 	rl_redisplay();
// 	g_status_code = 130;
// }

// TODO: Remove debug function
void	print_token(t_token token)
{
	char	*type;

	switch (token.type) {
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
		default:
			type = "Word";
			break ;
	}
	puts("{");
	printf("\tContent: %s,\n\tType: %s\n", token.content, type);
	puts("}");
}

/*
 * Loops until EOF is detected and reads user input using readline
 * and executes the command in a child process and finally
 * frees the user read line.
 * NOTE: Readline causes still reachable memory leaks
 */
int	main(int argc, char *argv[] __attribute__((unused)), char **env)
{
	char	*line;
	t_minishell minishell;
	t_cmd	*cmd;

	if (argc != 1)
		return (puts("Minishell can not run external files."), 1);
	g_status_code = 0;
	// signal(SIGINT, handle_sigint);
	minishell.prompt = init_prompt();
	line = readline(minishell.prompt->current);
	while (line)
	{
		add_history(line);
		parse(&minishell, line);
		cmd = create_exec_cmd(&minishell);
		run_cmd(cmd, env);
		free_cmd(cmd);
		free(line);
		free_tokens(&minishell);
		update_prompt(minishell.prompt);
		line = readline(minishell.prompt->current);
	}
	free(minishell.prompt->previous);
	free(minishell.prompt->current);
	free(minishell.prompt);
	puts("exit");
	return (g_status_code);
}
