/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maabdull <maabdull@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 14:47:16 by maabdull          #+#    #+#             */
/*   Updated: 2024/06/11 12:03:07 by maabdull         ###   ########.fr       */
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
		if (!ft_strncmp(str, builtins[i], ft_strlen(builtins[i]) + 1))
			return (true);
		i++;
	}
	return (false);
}

// int	ft_cd(char **cmd)
// {
// 	if (cmd[1])
// 		return (ft_putendl_fd("cd: too many arguments", 2), 1);
// 	chdir(cmd[0]);
// 	return (0);
// }
