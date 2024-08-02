/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdanish <mdanish@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 10:07:41 by mdanish           #+#    #+#             */
/*   Updated: 2024/08/01 16:39:39 by mdanish          ###   ########.fr       */
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
 * @param environment is used to create the matrix duplication from.
 */
static bool	create_matrix(t_minishell *minishell, char **environment)
{
	int	i;

	while (environment[minishell->envp_count])
		minishell->envp_count++;
	minishell->envp = ft_calloc(minishell->envp_count + 1, sizeof(char *));
	if (!minishell->envp)
		return (false);
	i = -1;
	while (++i < minishell->envp_count)
	{
		minishell->envp[i] = ft_strdup(environment[i]);
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
static char	**sort_environment_variables(char **environment, int env_count)
{
	char	*store;
	int		index_one;
	int		index_two;

	index_one = -1;
	while (++index_one < env_count)
	{
		index_two = index_one;
		while (++index_two < env_count)
		{
			if (ft_strncmp(environment[index_one], \
				environment[index_two], 100) > 0)
			{
				store = environment[index_two];
				environment[index_two] = environment[index_one];
				environment[index_one] = store;
			}
		}
	}
	return (environment);
}

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

	ft_memset(minishell, 0, sizeof(t_minishell));
	if (!create_matrix(minishell, env))
		return (g_code = 1, perror("Environment setup"));
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
