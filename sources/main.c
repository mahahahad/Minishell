/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maabdull <maabdull@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/31 13:43:49 by maabdull          #+#    #+#             */
/*   Updated: 2024/06/11 20:43:25 by maabdull         ###   ########.fr       */
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

// TODO: Remove debug function
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
		add_history(line);
		t_cmd *cmd = parse(&minishell, line);
		// cmd = create_exec_cmd(&minishell);
		run_cmd(cmd, env);
		// free_cmd(cmd);
		free(line);
		if (minishell.token_count)
		{
			for (int i = 0; i < minishell.token_count; i++){
					print_token(minishell.tokens[i]);
			}
			free_tokens(&minishell);
		}
		update_prompt(minishell.prompt);
		line = readline(minishell.prompt->current);
	}
	free(minishell.prompt->previous);
	free(minishell.prompt->current);
	free(minishell.prompt);
	ft_putendl_fd("exit", 1);
	return (g_status_code);
}
