/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdanish <mdanish@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 19:45:08 by mdanish           #+#    #+#             */
/*   Updated: 2024/05/29 13:06:30 by mdanish          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void add_to_matrix(t_minishell *minishell, char *new_var)
{
	char	**env_store;
	int		index;

	if (!ft_strchr(new_var, '=') || update_the_matrix(minishell, new_var))
		return ;
	env_store = minishell->envp;
	minishell->envp = malloc(sizeof(char *) * (minishell->envp_count + 2));
	if (!minishell->envp)
		ft_putendl_fd("Malloc failed while exporting a variable.", 2);	// exit required
	ft_memset(minishell->envp, 0, sizeof(char **));
	index = -1;
	while (++index < minishell->envp_count)
	{
		minishell->envp[index] = ft_strdup(env_store[index]);
		if (!minishell->envp[index])
			ft_putendl_fd("Malloc failed while exporting a variable.", 2);	// exit required
		free(env_store[index]);
	}
	free(env_store);
	minishell->envp[minishell->envp_count] = ft_strdup(new_var);
	if (!minishell->envp[index])
		ft_putendl_fd("Malloc failed while exporting a variable.", 2);	// exit required
	minishell->envp[++minishell->envp_count] = NULL;
}

static void	add_to_list(t_minishell *minishell, t_env_node *var, int len)
{
	t_env_node	*list;
	t_env_node	*store;

	list = minishell->env_variables;
	while (list->next)
	{
		if (check_for_existing_value(list, var, len + 1))
			return ;
		if (ft_strncmp(list->env_name, var->env_name, len + 1) > 0)
		{
			var->next = list;
			if (list == minishell->env_variables)
				minishell->env_variables = var;
			else
				store->next = var;
			return ;
		}
		store = list;
		list = list->next;
	}
	list->next = var;
}

static void	ft_print_export(t_env_node *env)
{
	while (env)
	{
		ft_putstr_fd("declare -x ", 1);
		ft_putstr_fd(env->env_name, 1);
		if (env->env_content)
		{
			ft_putstr_fd("=\"", 1);
			ft_putstr_fd(env->env_content, 1);
			ft_putchar_fd('\"', 1);
		}
		ft_putchar_fd('\n', 1);
		env = env->next;
	}
}

void	ft_export(t_minishell *minishell, char **new_variables)
{
	t_env_node	*var;
	int			length;

	g_status_code = 0;
	if (!new_variables)
	{
		ft_print_export(minishell->env_variables);
		return ;
	}
	while (*new_variables)
	{
		if (!is_argument_valid(*new_variables) && new_variables++)
			continue ;
		var = malloc(sizeof(t_env_node));
		if (!var)
			ft_putendl_fd("Malloc failed while exporting a variable.", 2);	// exit required
		ft_memset(var, 0, sizeof(t_env_node));
		create_new_variable(var, &length, *new_variables);
		add_to_list(minishell, var, length);
		add_to_matrix(minishell, *new_variables);
		new_variables++;
	}
}
