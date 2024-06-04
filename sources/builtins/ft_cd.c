/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdanish <mdanish@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 14:47:16 by maabdull          #+#    #+#             */
/*   Updated: 2024/06/05 00:44:08 by mdanish          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Checks the arguments for validity.
 * 
 * It checks whether there are multiple arguments or any options provided to cd.
 * 
 * If any of the two are detected, it prints an error message explaining the
 * error and sets g_status_code to 1.
 * 
 * If there are no arguments provided to cd, it checks through the list of
 * environment variables to identify the value stored in the HOME variable.
 * The value is then set as the argument for cd.
 * 
 * @param args contain the path to which the directory will be changed to.
 * @param list is the linked list of variable used to search for HOME directory.
 * 
 * @return true is invalid arguments are detected, false if arguments are valid.
 * 
 */

static bool	check_invalid_args(char **args, t_env *list)
{
	bool	error_flag;

	error_flag = false;
	if (args[1] && (args[2] || args[1][0] == '-'))
		error_flag = true;
	if (args[1] && args[2])
		ft_putendl_fd("cd: too many arguments", 2);
	else if (args[1] && args[1][0] == '-')
		ft_putendl_fd("cd: options are not accepted", 2);
	else if (!args[1])
	{
		while (list && !ft_strncmp(list->key, "HOME", 5))
			list = list->next;
		if (list)
			args[1] = list->value;
		else
		{
			ft_putendl_fd("cd: HOME not set", 2);
			error_flag = true;
		}
	}
	if (error_flag)
		g_status_code = 1;
	return (error_flag);
}

/**
 * @brief Creates the index for the env matrix.
 * 
 * This function allocates for a string that will contain the information for an
 * environment variable in the form of key="value".
 * 
 * @param key is the name of the environment variable.
 * @param value is the content of the environment variable.
 * @param total_length is the length of the final return string.
 * 
 * @return the final string matching the above form.
 * 
 */

static char	*make_value(char *key, char *value, int total_length)
{
	char	*matrix_index;
	int		index;

	index = 0;
	matrix_index = malloc(total_length + 1);
	if (!matrix_index)
		ft_putendl_fd("Malloc fail while changing directory", 2);	// exit requried
	while (index < total_length - 1 && *key)
		matrix_index[index++] = *key++;
	while (index < total_length - 1 && *value)
		matrix_index[index++] = *value++;
	matrix_index[index++] = '"';
	matrix_index[index] = '\0';
	return (matrix_index);
}

/**
 * @brief Changes the directory and updates env stores.
 * 
 * This function changes the directory using a function call to chdir(). In case
 * of failure, it prints an error message and set g_status_code to 1. Upon
 * success, it changes the node of the list if available and then uses
 * make_value() to create the argument for add_to_matrix(). Doing so will update
 * both the matrix and the list stores of the environment variables.
 * 
 * @param args contain the path to which the directory will be changed to.
 * @param old is a pointer to the node containing the env variable "OLDPWD".
 * @param new is a pointer to the node containing the env variable "PWD".
 * @param minishell is used to access the matrix and list to add variables.
 * 
 */

static void	chg_dir(char **args, t_env *old, t_env *new, t_minishell *minishell)
{
	char	*var;

	var = getcwd(NULL, 0);
	if (chdir(args[1]))
	{
		ft_putendl_fd("cd: can not change directory", 2);
		g_status_code = 1;
		return ;
	}
	if (old)
	{
		free(old->value);
		old->value = var;
		var = make_value("OLDPWD=\"", old->value, 9 + ft_strlen(old->value));
		add_to_matrix(minishell, var);
		free(var);
	}
	if (new)
	{
		free(new->value);
		new->value = getcwd(NULL, 0);
		var = make_value("PWD=\"", new->value, 6 + ft_strlen(new->value));
		add_to_matrix(minishell, var);
		free(var);
	}
}

/**
 * @brief Changes the current working directory
 * 
 * check_invalid_args() checks for multiple arguments and any options.
 * 
 * It identifies the OLDPWD node and PWD node in the env list and stores them in
 * local variables in the case that the change of the directory is successful.
 * 
 * Once stores have been made, the chg_dir() function changes the current
 * working directory.
 * 
 * Upon completion, it sets g_status_code to 0.
 * 
 * @param args contain the path to which the directory will be changed to.
 * @param minishell contains the env stores that will be updated on success.
 * 
 */

void	ft_cd(char **args, t_minishell *minishell)
{
	t_env	*old;
	t_env	*new;
	t_env	*list;

	old = NULL;
	new = NULL;
	list = minishell->env_variables;
	if (check_invalid_args(args, list))
		return ;
	g_status_code = 0;
	while (list)
	{
		if (!ft_strncmp(list->key, "OLDPWD", 7))
			old = list;
		else if (!ft_strncmp(list->key, "PWD", 4))
			new = list;
		list = list->next;
	}
	chg_dir(args, old, new, minishell);
}
