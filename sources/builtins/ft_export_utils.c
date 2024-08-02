/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdanish <mdanish@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 20:21:11 by mdanish           #+#    #+#             */
/*   Updated: 2024/08/01 15:56:01 by mdanish          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Checks if the new_var key exists in the matrix
 * 
 * If it finds the key to already exist within the matrix, it frees the index
 * and creates a duplicate of the new_var, which is then stored in that index.
 * If it isn't found, it will do nothing
 * 
 * @param minishell contains the matrix in which the key will be searched for.
 * @param new_variable contain the key that will be searched for.
 * 
 * @return true if the key exists, false if the key is not found in the matrix
 * 
 */
static bool	update_the_matrix(t_minishell *minishell, char *new_variable)
{
	int	index;
	int	length;

	index = -1;
	length = ft_strchr(new_variable, '=') - new_variable;
	while (++index < minishell->envp_count)
	{
		if (ft_strncmp(minishell->envp[index], new_variable, length + 1))
			continue ;
		free(minishell->envp[index]);
		minishell->envp[index] = ft_strdup(new_variable);
		if (!minishell->envp[index])
			return (perror("export"), g_code = 1, false);
		return (true);
	}
	return (false);
}

/**
 * @brief Adds the new node to the existing matrix of environment variables.
 * 
 * It first checks if the key exists already within the matrix using the
 * update_the_matrix() function. If it does not exist, it is added to the end of
 * the matrix be reallocating for a bigger matrix and then copying over from
 * the previous matrix.
 * 
 * Each index is copied over storing the previous pointer within the
 * corresponding index.
 * 
 * @param minishell is used to access the linked list of the variables.
 * @param new_variable is the new node that will be added to the linked list.
 *
 */
bool	add_to_matrix(t_minishell *minishell, char *new_variable)
{
	char	**env_store;
	int		index;

	if (!new_variable)
		return (false);
	if (!ft_strchr(new_variable, '=') || \
		update_the_matrix(minishell, new_variable))
		return (true);
	if (g_code)
		return (false);
	env_store = ft_calloc(minishell->envp_count + 2, sizeof(char *));
	if (!env_store)
		return (perror("export"), g_code = 1, false);
	index = -1;
	while (++index < minishell->envp_count)
		env_store[index] = minishell->envp[index];
	free(minishell->envp);
	minishell->envp = env_store;
	minishell->envp[minishell->envp_count] = ft_strdup(new_variable);
	if (!minishell->envp[index])
		return (perror("export"), g_code = 1, false);
	minishell->envp_count++;
	return (true);
}

/**
 * @brief Adds the information into the node.
 * 
 * It goes through the string to identify the key and uses ft_substr() to store
 * a copy of the key within the new_var node.
 * 
 * It then uses ft_substr() to create a copy of the value and also stores it in
 * the new_var node.
 * 
 * @param new_variable is the node within which new data will be stored in.
 * @param length is the address of the variable that will store key length.
 * @param string contains the new key and value to be added.
 * 
 */
void	create_new_variable(t_env *variable, int *length, char *string)
{
	if (!ft_strchr(string, '='))
	{
		variable->key = ft_strdup(string);
		if (!variable->key)
			return (g_code = 1, perror("export"));
		*length = ft_strlen(variable->key);
	}
	else
	{
		*length = ft_strchr(string, '=') - string;
		variable->key = ft_substr(string, 0, *length);
		if (!variable->key)
			return (g_code = 1, perror("export"));
		variable->value = ft_substr(string, *length + 1, \
				ft_strlen(string) - (*length + 1));
		if (!variable->value)
			return (g_code = 1, free(variable->key), variable->key = NULL, \
				perror("export"));
	}
}

/**
 * @brief Validates the argument.
 * 
 * It will go through the key part of the string to identify if any character
 * other than digits and alphabets, with the exception of the [ _ ], is used.
 * It will also check if the first character of the key is a digit.
 * 
 * If any of these tests are positive, an error message is printed and the
 * variable is not exported. Otherwise it does nothing.
 * 
 * @param string contain the key that needs to be tested.
 * 
 * @return true if valid, false if invalid.
 * 
 */
bool	is_argument_valid(const char *string)
{
	int	index;

	index = -1;
	while (string[++index] && string[index] != '=')
	{
		if (string[index] == '_')
			continue ;
		else if (ft_isalnum(string[index]))
			continue ;
		else
			break ;
	}
	if (ft_isdigit(string[0]) || !string[0] || \
		(string[index] && string[index] != '='))
	{
		ft_putendl_fd("Invalid identifier detected in the arguments.", 2);
		g_code = 1;
		return (false);
	}
	return (true);
}
