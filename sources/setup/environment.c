/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdanish <mdanish@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 10:07:41 by mdanish           #+#    #+#             */
/*   Updated: 2024/08/07 16:50:24 by mdanish          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	create_environment(t_minishell *minishell)
{
	char	*variable[3];
	char	*cwd;
	t_env	*env;

	ft_memset(variable, 0, sizeof(char *) * 3);
	env = minishell->env_variables;
	while (env && ft_strncmp(env->key, "OLDPWD", 7) && env->key[0] < 'P')
		env = env->next;
	variable[1] = "OLDPWD";
	if (env && env->key[0] < 'P' && env->value && access(env->value, 5) < 0)
		ft_unset(minishell, variable);
	else if (!env || env->key[0] > 'O')
		ft_export(minishell, variable);
	cwd = getcwd(NULL, 0);
	variable[1] = make_value("PWD=", cwd, 4 + ft_strlen(cwd), "env setup");
	ft_export(minishell, variable);
	free(variable[1]);
	while (env && ft_strncmp(env->key, "SHLVL", 6) && env->key[0] < 'T')
		env = env->next;
	if (!env || env->key[0] > 'S')
	{
		variable[1] = "SHLVL=1";
		ft_export(minishell, variable);
	}
}

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
	int	index;

	while (environment[minishell->envp_count])
		minishell->envp_count++;
	minishell->envp = ft_calloc(minishell->envp_count + 1, sizeof(char *));
	if (!minishell->envp)
		return (false);
	index = -1;
	while (++index < minishell->envp_count)
	{
		minishell->envp[index] = ft_strdup(environment[index]);
		if (!minishell->envp[index])
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
			if (ft_strncmp(environment[index_one],
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
	t_env	*variable;

	ft_memset(minishell, 0, sizeof(t_minishell));
	if (!create_matrix(minishell, env))
		return (g_code = 1, perror("Environment setup"));
	env = sort_environment_variables(env, minishell->envp_count);
	while (*env)
	{
		variable = ft_calloc(1, sizeof(t_env));
		if (!variable)
			break ;
		len = ft_strchr(*env, '=') - *env;
		variable->key = ft_substr(*env, 0, len);
		if (!variable->key)
			break ;
		variable->value = ft_substr(*env, len + 1, ft_strlen(*env) - len - 1);
		if (!variable->value)
			break ;
		ft_lstadd_back(&minishell->env_variables, variable);
		env++;
	}
	if (!*env)
		return (create_environment(minishell));
	free_environment(minishell);
}
