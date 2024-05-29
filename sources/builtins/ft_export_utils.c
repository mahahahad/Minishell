/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdanish <mdanish@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 20:21:11 by mdanish           #+#    #+#             */
/*   Updated: 2024/05/29 12:41:38 by mdanish          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	update_the_matrix(t_minishell *minishell, char *new_var)
{
	int	index;
	int	length;

	index = -1;
	length = ft_strchr(new_var, '=') - new_var;
	while (++index < minishell->envp_count)
	{
		char *store = minishell->envp[index];
		if (ft_strncmp(store, new_var, length + 1))
			continue ;
		free( minishell->envp[index]);
		minishell->envp[index] = malloc(ft_strlen(new_var) + 1);
		if (!minishell->envp[index])
			ft_putendl_fd("Malloc failed while exporting a variable.", 2);	// exit required
		ft_strlcpy(minishell->envp[index], new_var, ft_strlen(new_var) + 1);
		return (true);
	}
	return (false);
}

bool	check_for_existing_value(t_env_node *list, t_env_node *var, int len)
{
	if (!ft_strncmp(list->env_name, var->env_name, len))
	{
		if (var->env_content)
		{
			free(list->env_content);
			list->env_content = var->env_content;
			free(var->env_name);
			free(var);
		}
		return (true);
	}
	return (false);
}

void	create_new_variable(t_env_node *new_var, int *length, char *string)
{
	if (!ft_strchr(string, '='))
	{
		new_var->env_name = ft_strdup(string);
		if (!new_var->env_name)
			ft_putendl_fd("Malloc failed while exporting a variable.", 2);	// exit required
		*length = ft_strlen(new_var->env_name);
	}
	else
	{
		*length = ft_strchr(string, '=') - string;
		new_var->env_name = ft_substr(string, 0, *length);
		if (!new_var->env_name)
			ft_putendl_fd("Malloc failed while exporting a variable.", 2);	// exit required
		new_var->env_content = ft_substr(string, *length + 1, \
				ft_strlen(string) - (*length + 1));
		if (!new_var->env_content)
			ft_putendl_fd("Malloc failed while exporting a variable.", 2);	// exit required
	}
}

bool	is_argument_valid(const char *string)
{
	int	index;

	index = -1;
	while (string[++index] && string[index] != '=')
	{
		if (string[index] == '_')
			continue ;
		else if (string[index] >= '0' && string[index] <= '9')
			continue ;
		else if ((string[index] >= 'A' && string[index] <= 'Z') || \
			(string[index] >= 'a' && string[index] <= 'z'))
			continue ;
		else
			break ;
	}
	if ((string[0] >= '0' && string[0] <= '9') || !string[0] || \
		(string[index] && string[index] != '='))
	{
		ft_putendl_fd("Invalid identifier detected in the arguments.", 2);
		g_status_code = 1;
		return (false);
	}
	return (true);
}
