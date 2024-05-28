/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdanish <mdanish@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 14:47:16 by maabdull          #+#    #+#             */
/*   Updated: 2024/05/28 19:05:38 by mdanish          ###   ########.fr       */
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

void	ft_pwd(char **args)
{
	char	*current_working_directory;
	if (args && *args && **args)
	{
		write(2, "pwd does not accecpt options or arguments.\n", 43);
		return ;
	}
	current_working_directory = getcwd(NULL, 0);
	printf("%s\n", current_working_directory);
	free(current_working_directory);
	g_status_code = 0;
}

void	ft_env(char **envp)
{
	while (*envp)
		printf("%s\n", *envp++);
	g_status_code = 0;
}

void	ft_unset_from_list(t_minishell *minishell, char *variable, int var_len)
{
	t_env_node	*delnode;
	t_env_node	*store;

	delnode = minishell->env_variables;
	if (!ft_strncmp(variable, delnode->env_name, var_len))
		minishell->env_variables = minishell->env_variables->next;
	else
	{
		store = minishell->env_variables;
		while (delnode)
		{
			delnode = delnode->next;
			if (delnode && !ft_strncmp(variable, delnode->env_name, var_len))
				break ;
			store = store->next;
		}
		if (!delnode)
			return ;
		store->next = delnode->next;
	}
	free(delnode->env_name);
	free(delnode->env_content);
	free(delnode);
}

void	ft_unset(t_minishell *minishell, char *variable)
{
	int			var_len;
	int			index;

	index = -1;
	var_len = ft_strlen(variable) + 1;
	while (++index != minishell->envp_count)
		if (!ft_strncmp(variable, minishell->envp[index], var_len))
			break ;
	if (index != minishell->envp_count)
	{
		free(minishell->envp[index]);
		minishell->envp_count--;
		while (index <= minishell->envp_count)
		{
			minishell->envp[index] = minishell->envp[index + 1];
			index++;
		}
	}
	ft_unset_from_list(minishell, variable, var_len);
	g_status_code = 0;
}

void	ft_echo(char **cmd)
{
	int	index;

	index = 1;
	if (!ft_strncmp(cmd[1], "-n", 3))
		index++;
	while (cmd[index])
	{
		write(1, cmd[index], ft_strlen(cmd[index]));
		if (cmd[++index])
			write(1, " ", 1);
	}
	if (ft_strncmp(cmd[1], "-n", 3))
		write(1, "\n", 1);
	g_status_code = 0;
}

int	ft_cd(char **cmd)
{
	if (cmd[1])
		return (fprintf(stderr, "cd: too many arguments\n"), 1);
	chdir(cmd[0]);
	return (0);
}
