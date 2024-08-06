/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdanish <mdanish@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 14:47:16 by maabdull          #+#    #+#             */
/*   Updated: 2024/08/06 21:41:12 by mdanish          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	expand_tilde(t_env *home, char **arguments, int arg_length)
{
	char	*expanded;
	int		val_len;

	if (!arguments[1])
	{
		free(arguments[0]);
		arguments[0] = ft_strdup(home->value);
		if (!arguments[0])
			return (perror("cd"), true);
	}
	else if (!ft_strncmp(arguments[1], "~", 2) || \
		!ft_strncmp(arguments[1], "~/", 2))
	{
		val_len = ft_strlen(home->value);
		expanded = ft_calloc(arg_length + val_len, sizeof(char));
		if (!expanded)
			return (perror("cd"), true);
		ft_memcpy(expanded, home->value, val_len);
		ft_memcpy(expanded + val_len, arguments[1] + 1, arg_length);
		free(arguments[1]);
		arguments[1] = expanded;
	}
	return (false);
}

/**
 * @brief Checks the arguments for validity.
 * 
 * It checks whether there are multiple arguments or any options provided to cd.
 * 
 * If any of the two are detected, it prints an error message explaining the
 * error and sets g_code to 1.
 * 
 * If there are no arguments provided to cd, it checks through the list of
 * environment variables to identify the value stored in the HOME variable.
 * The value is then set as the argument for cd.
 * 
 * @param arguments contain the path to which the directory will be changed to.
 * @param environment is the list of variable used to search for HOME directory.
 * 
 * @return true is invalid arguments are detected, false if arguments are valid.
 * 
 */
static bool	check_invalid_args(char **arguments, t_env *environment)
{
	bool	error_flag;

	error_flag = false;
	if (arguments[1] && (arguments[2] || arguments[1][0] == '-'))
		error_flag = true;
	if (arguments[1] && arguments[2])
		ft_putendl_fd("cd: too many arguments", 2);
	else if (arguments[1] && arguments[1][0] == '-')
		ft_putendl_fd("cd: options are not accepted", 2);
	while (environment && ft_strncmp(environment->key, "HOME", 5))
		environment = environment->next;
	if ((!environment && (!arguments[1] || !ft_strncmp(arguments[1], "~", 2)
		|| !ft_strncmp(arguments[1], "~/", 2))) || !environment->value)
		return (ft_putendl_fd("cd: HOME not set", 2), true);
	error_flag = expand_tilde(environment, arguments, ft_strlen(arguments[1]));
	return (g_code = error_flag, error_flag);
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
	matrix_index = ft_calloc(total_length + 1, sizeof(char));
	if (!matrix_index)
		return (perror("cd"), g_code = 1, NULL);
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
 * of failure, it prints an error message and set g_code to 1. Upon
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
static void	chg_dir(char **args, char **old, char **new, t_minishell *minishell)
{
	char	*var;
	char	*export[4];

	var = getcwd(NULL, 0);
	if (!var || (args[1] && chdir(args[1])) || (!args[1] && chdir(args[0])))
		return (g_code = 1, perror("cd"), free(var));
	ft_memset(export, 0, 4 * sizeof(char *));
	export[1] = make_value("OLDPWD=\"", var, 9 + ft_strlen(var));
	if (old)
		free(*old);
	if (old)
		*old = var;
	else
		free(var);
	var = getcwd(NULL, 0);
	export[2] = make_value("PWD=\"", var, 6 + ft_strlen(var));
	if (new)
		free(*new);
	if (new)
		*new = var;
	else
		free(var);
	ft_export(minishell, export);
	free(export[1]);
	free(export[2]);
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
 * Upon completion, it sets g_code to 0.
 * 
 * @param args contain the path to which the directory will be changed to.
 * @param minishell contains the env stores that will be updated on success.
 * 
 */
void	ft_cd(char **arguments, t_minishell *minishell)
{
	char	**old;
	char	**new;
	t_env	*list;

	old = NULL;
	new = NULL;
	list = minishell->env_variables;
	if (check_invalid_args(arguments, list))
		return ;
	g_code = 0;
	while (list)
	{
		if (!ft_strncmp(list->key, "OLDPWD", 7))
			old = &list->value;
		else if (!ft_strncmp(list->key, "PWD", 4))
			new = &list->value;
		list = list->next;
	}
	chg_dir(arguments, old, new, minishell);
}
