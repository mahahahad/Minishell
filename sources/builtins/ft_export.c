/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdanish <mdanish@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 19:45:08 by mdanish           #+#    #+#             */
/*   Updated: 2024/06/26 16:45:12 by mdanish          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Checks if the node key exists in the list
 * 
 * If it finds the key to already exist within the list, it ends up freeing the
 * previous value, replacing it with the new value and then freeing the rest of
 * the new node. This is only done if there is a new value.
 * 
 * In the case that the key exists but the value is NULL, nothing happens.
 * 
 * @param list is the linked list in which the key will be searched for.
 * @param var is the new node of which the key will be searched for.
 * @param len is the length of the key of the new node.
 * 
 * @return true if the key exists, false if the key is not found in the list
 * 
 */
static bool	check_for_existing_value(t_env *list, t_env *var, int len)
{
	if (!ft_strncmp(list->key, var->key, len))
	{
		if (var->value)
		{
			free(list->value);
			list->value = var->value;
			free(var->key);
			free(var);
		}
		return (true);
	}
	return (false);
}

/**
 * @brief Adds the new node to the existing list of environment variables.
 * 
 * It first checks if the key in the new node exists already within the list
 * using the check_for_existing_value() function. If it does not exist, it is
 * added by identifying the correct place within the list according to the
 * alphabetical order of the variables.
 * 
 * Once identified, it changes the pointers of the surrounging nodes to ensure
 * that the new node is properly added to the list.
 * 
 * @param minishell is used to access the linked list of the variables.
 * @param var is the new node that will be added to the linked list.
 * @param len is the length of the key of the new node.
 * 
 */
static void	add_to_list(t_minishell *minishell, t_env *var, int len)
{
	t_env	*list;
	t_env	*store;

	list = minishell->env_variables;
	while (list->next)
	{
		if (check_for_existing_value(list, var, len + 1))
			return ;
		if (ft_strncmp(list->key, var->key, len + 1) > 0)
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

/**
 * @brief Prints the list of variables.
 * 
 * It goes through the linked list og env variables and prints them according
 * to whethter there is a content or not using the put_fd functions.
 * 
 * @param env is the linked list of variables that will be printed to stdout.
 * 
 */
static void	ft_print_export(t_env *env)
{
	while (env)
	{
		ft_putstr_fd("declare -x ", 1);
		ft_putstr_fd(env->key, 1);
		if (env->value)
		{
			ft_putstr_fd("=\"", 1);
			ft_putstr_fd(env->value, 1);
			ft_putchar_fd('\"', 1);
		}
		ft_putchar_fd('\n', 1);
		env = env->next;
	}
}

/**
 * @brief Adds new variables to the environment. / Prints the list of variables.
 * 
 * If new variables is NULL, it will print the list through ft_print_export().
 * 
 * For each varable, it identifies whether the variable abides by the rules of
 * variable names. This is done by sending it to the is_argument_valid()
 * function. If valid, it will create a new node and store the within it the
 * key and value of the variable.
 * 
 * It then sends it to the add_to_list() and add_to_matrix() functions to add
 * that node to the list and matrix respectively.
 * 
 * Upon completion, it sets g_status_code to 0.
 * 
 * @param minishell is used to access the matrix and list to add variables.
 * @param new_variables will be exported individually. (NULL terminated)
 * 
 */
void	ft_export(t_minishell *minishell, char **new_variables)
{
	t_env	*var;
	int		length;

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
		var = ft_calloc(1, sizeof(t_env));
		if (!var)
			ft_putendl_fd("Malloc failed while exporting a variable.", 2);	// exit required
		create_new_variable(var, &length, *new_variables);
		add_to_list(minishell, var, length);
		add_to_matrix(minishell, *new_variables);
		new_variables++;
	}
}
