/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdanish <mdanish@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 14:47:16 by maabdull          #+#    #+#             */
/*   Updated: 2024/05/15 15:17:59 by mdanish          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void	ft_env(char **envp)
{
	while (*envp)
		printf("%s\n", *envp++);
	g_status_code = 0;
}

// TODO: Fix this temporary solution
void		ft_echo(char **cmd)
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

int	ft_cd(char **cmd)
{
	if (cmd[1])
		return (fprintf(stderr, "cd: too many arguments\n"), 1);
	chdir(cmd[0]);
	return (0);
}
