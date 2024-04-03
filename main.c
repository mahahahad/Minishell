/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdanish <mdanish@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/31 13:43:49 by maabdull          #+#    #+#             */
/*   Updated: 2024/04/03 18:31:10 by mdanish          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Creates a child process and executes the specified command 
 * Doesn't accept parameters to commands right now
 */
int	exec_cmd(char *cmd)
{
	char	*null_dbl_ptr[] = {NULL};
	int	status;
	int	pid;

	pid = fork();
	if (pid == 0)
	{
		execve(cmd, null_dbl_ptr, null_dbl_ptr);
		perror("execve");
		exit(1);
	}
	waitpid(pid, &status, 0);
	return (status);
}

/*
 * Loops until EOF is detected and reads user input using readline
 * and executes the command in a child process and finally
 * frees the user read line. 
 * NOTE: Readline causes still reachable memory leaks
 */
int	main(void)
{
	char	*line;

	line = readline(PROMPT);
	while (line)
	{
		exec_cmd(line);
		free(line);
		line = readline(PROMPT);
	}
	rl_clear_history();
	return (0);
}
