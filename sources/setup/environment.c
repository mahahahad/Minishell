/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdanish <mdanish@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 10:07:41 by mdanish           #+#    #+#             */
/*   Updated: 2024/06/04 20:59:25 by mdanish          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Creates the matrix duplicate of the environment variables.
 * 
 * It is done by mallocing for a char ** and the running ft_strdup() on each
 * index of env and it is then stored in the newly allocated matrix.
 * 
 * @param minishell is used to store the matrix duplication.
 * @param env is used to create the matrix duplication from.
 */

static void	create_matrix(t_minishell *minishell, char **env)
{
	int	i;

	while (env[minishell->envp_count])
		minishell->envp_count++;
	minishell->envp = malloc((sizeof(char *) * (minishell->envp_count + 1)));
	if (!minishell->envp)
		ft_putendl_fd("Malloc failed while setting up the env variables", 2);	// exit required
	ft_memset(minishell->envp, 0, sizeof(char **));
	i = -1;
	while (++i < minishell->envp_count)
	{
		minishell->envp[i] = ft_strdup(env[i]);
		if (!minishell->envp[i])
			ft_putendl_fd("Malloc failed while setting up the env variables", 2);	// exit required
	}
	minishell->envp[minishell->envp_count] = NULL;
}

/**
 * @brief Sorts the list of the environment variables into alphabetical order.
 * 
 * It uses an implementation of bubble sort to achieve the result.
 * 
 * @param env is the list of environment variables that need to be sorted.
 * @param env_count is the total number of the environment variables.
 * 
 * @return the final sorted matrix of the env.
 */

static char	**sort_environment_variables(char **env, int env_count)
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
			if (ft_strncmp(env[i], env[j], 100) > 0)
			{
				store = env[j];
				env[j] = env[i];
				env[i] = store;
			}
		}
	}
	return (env);
}

// We still need to come up with a proper cleanup.
// The cleanup will require a listclear function as well.

/**
 * @brief Creates a duplicate of the environment variable in the form of a
 * char ** matrix as well as a linked list.
 * 
 * It uses the create_matrix() function to create the char ** matrix and then
 * creates a linked list of the * variables after the variables are sorted by
 * sort_environment_variables().
 * 
 * @param minishell is used to store the final duplicates.
 * @param env is used to create the duplicates from.
 *
 */

void	setup_environment(t_minishell *minishell, char **env)
{
	int		len;
	t_env	*var;

	create_matrix(minishell, env);
	env = sort_environment_variables(env, minishell->envp_count);
	while (*env)
	{
		var = malloc(sizeof(t_env));
		if (!var)
			break ;
		ft_memset(var, 0, sizeof(t_env));
		len = ft_strchr(*env, '=') - *env;
		var->key = ft_substr(*env, 0, len);
		if (!var->key)
			break ;
		var->value = ft_substr(*env, len + 1, ft_strlen(*env) - len - 1);
		if (!var->value)
			break ;
		ft_lstadd_back(&minishell->env_variables, var);
		env++;
	}
	if (!*env)
		return ;
	ft_putendl_fd("Malloc failed while setting up the env variables", 2);	// exit required
}
