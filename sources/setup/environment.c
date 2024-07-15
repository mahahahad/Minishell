/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdanish <mdanish@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 10:07:41 by mdanish           #+#    #+#             */
/*   Updated: 2024/07/15 12:52:04 by mdanish          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Cleanup of the environment in case of any malloc fail.
 * 
 * The function calls free_split() on the char double pointer version of the
 * environment variables. The list version of it is freed node by node along
 * with the contents of it. Both versions are set to NULL after freeing.
 * 
 * @param minishell contains the environment that needs to be freed.
 */
static void	free_environment(t_minishell *minishell)
{
	t_env	*store;

	free_split(minishell->envp, minishell->envp_count);
	minishell->envp = NULL;
	while (minishell->env_variables)
	{
		store = minishell->env_variables->next;
		free(minishell->env_variables->key);
		free(minishell->env_variables->value);
		free(minishell->env_variables);
		minishell->env_variables = store;
	}
	minishell->env_variables = NULL;
}

/**
 * @brief Creates the matrix duplicate of the environment variables.
 * 
 * It is done by mallocing for a char ** and the running ft_strdup() on each
 * index of env and it is then stored in the newly allocated matrix.
 * 
 * @param minishell is used to store the matrix duplication.
 * @param env is used to create the matrix duplication from.
 */
static bool	create_matrix(t_minishell *minishell, char **env)
{
	int	i;

	while (env[minishell->envp_count])
		minishell->envp_count++;
	minishell->envp = ft_calloc(minishell->envp_count + 1, sizeof(char *));
	if (!minishell->envp)
		return (false);
	i = -1;
	while (++i < minishell->envp_count)
	{
		minishell->envp[i] = ft_strdup(env[i]);
		if (!minishell->envp[i])
			return (free_split(minishell->envp, minishell->envp_count), false);
	}
	minishell->envp[minishell->envp_count] = NULL;
	return (true);
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

	if (!create_matrix(minishell, env))
		return (ft_putendl_fd("Malloc fail in env variables set up.", 2));
	env = sort_environment_variables(env, minishell->envp_count);
	while (*env)
	{
		var = ft_calloc(1, sizeof(t_env));
		if (!var)
			break ;
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
	free_environment(minishell);
}
