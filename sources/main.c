/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maabdull <maabdull@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/31 13:43:49 by maabdull          #+#    #+#             */
/*   Updated: 2024/05/03 00:57:34 by maabdull         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

int		g_status_code;

char	*ft_char_strjoin(char const *s1, char const *s2, char const c)
{
	char	*final_str;
	int		i;
	int		j;

	i = -1;
	j = 0;
	if (!s1 || !s2)
		return (NULL);
	final_str = malloc(ft_strlen(s1) + ft_strlen(s2) + 2);
	if (!final_str)
		return (NULL);
	while (s1[++i])
		final_str[i] = s1[i];
	final_str[i++] = c;
	while (s2[j])
		final_str[i++] = s2[j++];
	final_str[i] = '\0';
	return (final_str);
}

// Mallocs the full path to a command
char	*find_cmd(char *cmd)
{
	char	*final_cmd;
	char	**dirs;
	char	*path;
	int		j;

	final_cmd = NULL;
	path = getenv("PATH");
	if (!path)
		return (NULL);
	dirs = ft_split(path, ':');
	j = 0;
	while (dirs[j])
	{
		final_cmd = ft_char_strjoin(dirs[j], cmd, '/');
		if (access(final_cmd, X_OK) == 0)
			break ;
		// free(final_cmd);
		j++;
	}
	return (final_cmd);
}

/*
 * Creates a child process and executes the specified command
 * Accepts an array of strings as input that will be passed to execve
 * following the format:
 * {command, options}
 */
int	exec_cmd(char **cmd, char **env)
{
	int		status;
	int		pid;
	char	*absolute_cmd;
	char	*cmd_original;

	cmd_original = cmd[0];
	if (!ft_strchr(cmd[0], '/'))
	{
		absolute_cmd = find_cmd(cmd[0]);
		cmd[0] = absolute_cmd;
	}
	else
	{
		if (access(cmd[0], F_OK) == -1)
			return (fprintf(stderr, "%s: no such file or directory\n",
					cmd_original), 127);
		else if (access(cmd[0], X_OK) == -1)
			return (fprintf(stderr, "%s: permission denied\n", cmd_original),
				126);
	}
	pid = fork();
	if (pid == 0)
	{
		execve(cmd[0], cmd, env);
		fprintf(stderr, "%s: command not found\n", cmd_original);
		free(cmd_original);
		exit(127);
	}
	waitpid(pid, &status, 0);
	free(cmd_original);
	free(absolute_cmd);
	return (status);
}

void	handle_sigint(int signum)
{
	(void)signum;
	puts("");
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	g_status_code = 130;
}

// store output from whoami and uname -n
// instead of envariables
char	*update_prompt(void)
{
	int		i;
	char	*temp;
	char	*prompt;
	char	*home_dir;
	char	*current_dir;

	prompt = YELLOW "msh" RESET "[" B_YELLOW;
	current_dir = getcwd(NULL, 0);
	home_dir = getenv("HOME");
	i = 0;
	if (home_dir && current_dir)
	{
		i = ft_strlen(home_dir);
		if (ft_strncmp(current_dir, home_dir, i) == 0)
		{
			prompt = ft_strjoin(prompt, "~");
			temp = prompt;
			prompt = ft_strjoin(prompt, current_dir + i);
			free(temp);
		}
		else
			prompt = ft_strjoin(prompt, current_dir + i);
	}
	else
		prompt = ft_strjoin(prompt, current_dir + i);
	temp = prompt;
	prompt = ft_strjoin(prompt, RESET "] ");
	free(temp);
	free(current_dir);
	return (prompt);
}

int	count_quotations(char *line)
{
	int	count;
	int	i;

	i = 0;
	count = 0;
	while (line[i])
	{
		if (line[i] == '"')
			count++;
		i++;
	}
	return (count);
}

/**
 * @brief Counts how many tokens there are in the provided input string
 * Uses spaces as a delimiter but ignores them in quoted strings
 */
int	count_tokens(char *input)
{
	int		i;
	int		j;
	bool	space_found;
	bool	quotes_found;

	i = 0;
	j = 0;
	quotes_found = false;
	space_found = false;
	while (input[i])
	{
		if (input[i] == '"' || input[i] == '\'')
			quotes_found = !quotes_found;
		if (!quotes_found && !space_found && input[i] == ' ')
		{
			if (input[i + 1] && input[i + 1] != ' ')
			{
				j++;
				space_found = true;
			}
		}
		if (!quotes_found && input[i] != ' ')	
			space_found = false;
		i++;
	}
	if (i > 0)
		j++;
	return (j);
}

/*
 * Reference input:
 * "a b c"
 *
 * Calling get_token on this should return:
 * "a b c\0"
 */
char	*get_token(char	*input)
{
	int	i;
	char	*token;
	bool	quotes_found;
	bool	space_found;

	i = 0;
	quotes_found = false;
	space_found = false;
	// puts(input);
	while (input[i])
	{
		if (input[i] == '"' || input[i] == '\'')
			quotes_found = !quotes_found;
		if (!quotes_found && !space_found && i > 0 && input[i] == ' ')
		{
			space_found = true;
			break ;
			// if (input[i + 1] && input[i + 1] != ' ') { }
		}
		if (!quotes_found && input[i] != ' ')	
			space_found = false;
		i++;
	}
	token = malloc((i));
	if (!token)
		return (NULL);
	strncpy(token, input, i);
	token[i] = '\0';
	return (token);
}

char	**tokenize(char *input)
{
	char	**tokens;
	int	i;
	int	token_count;
	int	token_length;

	i = 0;
	token_length = 0;
	token_count = count_tokens(input);
	tokens = malloc(token_count + 1);
	while (i < token_count)
	{
		tokens[i] = get_token(input + token_length);
		// puts(tokens[i]);
		token_length += ft_strlen(tokens[i]) + 1;
		i++;
	}
	tokens[i] = NULL;
	return (tokens);
}

void	parse(t_minishell *minishell, char *line)
{
	if (count_quotations(line) % 2 != 0)
		fprintf(stderr, RED "Open quotes detected, command rejected.\n" RESET);
	minishell->tokens = tokenize(line);
	ft_printarr(minishell->tokens);
}

bool	is_builtin(char *str)
{
	char	*builtins[8];
	int		i;

	i = 0;
	builtins[0] = "echo";
	builtins[1] = "cd";
	builtins[2] = "pwd";
	builtins[3] = "export";
	builtins[4] = "unset";
	builtins[5] = "env";
	builtins[6] = "exit";
	builtins[7] = NULL;
	while (builtins[i])
	{
		if (ft_strncmp(str, builtins[i], ft_strlen(builtins[i])) == 0)
			return (true);
		i++;
	}
	return (false);
}

// TODO: FIX THIS TEMPORARY SOLUTION
void	echo(char **cmd)
{
	int		i;
	bool	display_newline;

	i = 0;
	display_newline = true;
	if (ft_strncmp(cmd[i], "-n", 2) == 0)
	{
		i++;
		display_newline = false;
	}
	while (cmd[i])
	{
		printf("%s ", cmd[i]);
		i++;
	}
	if (display_newline)
		puts("");
}

int	cd(char **cmd)
{
	if (cmd[1])
		return (fprintf(stderr, "cd: too many arguments\n"), 1);
	chdir(cmd[0]);
	return (0);
}

void	exec_builtin(char **cmd)
{
	if (ft_strncmp(cmd[0], "echo", 4) == 0)
		echo(cmd + 1);
	if (ft_strncmp(cmd[0], "cd", 2) == 0)
		g_status_code = cd(cmd + 1);
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
	char	*prompt;
	t_minishell minishell;

	(void)env;
	if (argc != 1)
		return (puts("Minishell can not run external files."), 1);
	g_status_code = 0;
	signal(SIGINT, handle_sigint);
	prompt = update_prompt();
	line = readline(prompt);
	while (line)
	{
		add_history(line);
		parse(&minishell, line);
		// tokens = ft_split(line, ' ');
		// if (is_builtin(tokens[0]))
		// 	exec_builtin(tokens);
		// else
		// 	exec_cmd(tokens, env);
		free(line);
		ft_free_2d_arr(minishell.tokens);
		prompt = update_prompt();
		line = readline(prompt);
	}
	puts("exit");
	return (g_status_code);
}
