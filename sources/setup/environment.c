/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdanish <mdanish@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 10:07:41 by mdanish           #+#    #+#             */
/*   Updated: 2024/05/15 13:34:32 by mdanish          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	create_matrix(t_minishell *minishell, char **env)
{
	int	i;

	while ((env + minishell->envp_count))
		minishell->envp_count++;
	minishell->envp = malloc((sizeof(char *) * minishell->envp_count) + 1);
	if (!minishell->envp)
		write(2, "Malloc failed while setting up the env variables\n", 49);
	ft_memset(minishell->envp, 0, sizeof(char **));
	i = -1;
	while (++i < minishell->envp_count)
	{
		*(minishell->envp + i) = ft_strdup(*(env + i));
		if (!*(minishell->envp + i))
			write(2, "Malloc failed while setting up the env variables\n", 49);
	}
	*(minishell->envp + minishell->envp_count) = NULL;
}

char	**sort_environment_variables(char **env, int env_count)
{
	char	*store;
	int		i;
	int		j;

	i = -1;
	while (++i < env_count)
	{
		j = i;
		while (++j < env_count)
		{
			if (ft_strncmp(*(env + i), *(env + j), 100) > 0)
			{
				store = *(env + j);
				*(env + j) = *(env + i);
				*(env + i) = store;
			}
		}
	}
	return (env);
}

// We still need to come up with a proper cleanup.
// The cleanup will require a listclear function as well.
void	setup_environment(t_minishell *minishell, char **env)
{
	int			len;
	t_env_node	*var;

	create_matrix(minishell, env);
	env = sort_environment_variables(env, minishell->envp_count);
	while (*env)
	{
		var = malloc(sizeof(t_env_node));
		if (!var)
			write(2, "Malloc failed while setting up the env variables\n", 49);
		ft_memset(var, 0, sizeof(t_env_node *));
		len = ft_strchr(*env, '=') - *env;
		var->env_name = ft_substr(*env, 0, len);
		if (!var->env_name)
			write(2, "Malloc failed while setting up the env variables\n", 49);
		var->env_content = ft_substr(*env, len + 1, ft_strlen(*env) - len - 1);
		if (!var->env_content)
			write(2, "Malloc failed while setting up the env variables\n", 49);
		var->env_print = true;
		ft_lstadd_back(&minishell->env_variables, var);
		env++;
	}
	// for (t_env_node *i = minishell->env_variables; i; i = i->next){
	// 	puts("{");
	// 	printf("\tType: %s\n\tContent: %s\n", "NAME", i->env_name);
	// 	printf("\tType: %s\n\tContent: %s\n", "VALUE", i->env_content);
	// 	puts("}\n");
	// }
}
