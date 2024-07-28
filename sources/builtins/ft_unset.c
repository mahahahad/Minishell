/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdanish <mdanish@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 13:34:56 by mdanish           #+#    #+#             */
/*   Updated: 2024/07/27 19:11:40 by mdanish          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Deletes a variable from the linked list.
 * 
 * 
 * It goes through the list to identify the node to be deleted. If it is the
 * first node, the pointer in minishell is updated to point to the next of the
 * node that will be deleted. If it is not the first one, upon identification,
 * it changes the next-pointer of the previous node to point to the node that
 * comes after the one to be deleted.
 * 
 * Once the pointers are updated, it frees the content of the node and then
 * the node itself.
 * 
 * @param minishell is used to access the list to delete the variable.
 * @param var is the variable that needs to be unset.
 * @param var_len is the length of the variable that needs to be unset.
 *
 */
static void	ft_unset_from_list(t_minishell *minishell, char *var, int var_len)
{
	t_env	*delnode;
	t_env	*store;

	delnode = minishell->env_variables;
	if (!ft_strncmp(var, delnode->key, var_len))
		minishell->env_variables = minishell->env_variables->next;
	else
	{
		store = minishell->env_variables;
		while (delnode)
		{
			delnode = delnode->next;
			if (delnode && !ft_strncmp(var, delnode->key, var_len))
				break ;
			store = store->next;
		}
		if (!delnode)
			return ;
		store->next = delnode->next;
	}
	free(delnode->key);
	free(delnode->value);
	free(delnode);
}

/**
 * @brief Deletes variables from the stores of the environment variables.
 * 
 * For each varable, it goes through the matrix and frees the matching index.
 * It then replaces the current pointer with the pointer of the next variable
 * to get rid of the need for reallocating the entire matrix.
 * 
 * After that, it uses ft_unset_from_list to delete the matching node from the
 * linked list of variables.
 * 
 * Upon completion, it sets g_code to 0.
 * 
 * @param minishell is used to access the matrix and list to delete variables.
 * @param variable are the variables that need to be unset. (NULL terminated)
 *
 */
void	ft_unset(t_minishell *minishell, char **variable)
{
	int			var_len;
	int			index;

	while (*variable)
	{
		index = -1;
		var_len = ft_strlen(*variable) + 1;
		while (++index != minishell->envp_count)
			if (!ft_strncmp(*variable, minishell->envp[index], var_len))
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
		ft_unset_from_list(minishell, *variable, var_len);
		variable++;
	}
	g_code = 0;
}
