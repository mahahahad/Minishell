/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdanish <mdanish@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 14:47:16 by maabdull          #+#    #+#             */
/*   Updated: 2024/08/08 19:00:51 by mdanish          ###   ########.fr       */
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
static bool	check_invalid_args(char **arguments, t_env *variables)
{
	bool	error_flag;

	error_flag = false;
	if (arguments[1] && arguments[2])
		ft_putendl_fd("cd: too many arguments", 2);
	else if (arguments[1] && arguments[1][0] == '-')
		ft_putendl_fd("cd: options are not accepted", 2);
	if (arguments[1] && (arguments[2] || arguments[1][0] == '-'))
		return (g_code = 1, true);
	while (variables && ft_strncmp(variables->key, "HOME", 5))
		variables = variables->next;
	if ((!variables && (!arguments[1] || !ft_strncmp(arguments[1], "~", 2) || \
	!ft_strncmp(arguments[1], "~/", 2))) || (variables && !variables->value))
		return (ft_putendl_fd("cd: HOME not set", 2), true);
	error_flag = expand_tilde(variables, arguments, ft_strlen(arguments[1]));
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
char	*make_value(char *key, char *cwd, int total_length, char *err)
{
	char	*matrix_index;
	int		index_one;
	int		index_two;

	index_one = 0;
	if (!cwd)
		return (perror(err), g_code = WEXITSTATUS(errno), NULL);
	matrix_index = ft_calloc(total_length + 1, sizeof(char));
	if (!matrix_index)
		return (perror(err), g_code = 1, NULL);
	while (index_one < total_length && *key)
		matrix_index[index_one++] = *key++;
	index_two = -1;
	while (index_one < total_length && cwd[++index_two])
		matrix_index[index_one++] = cwd[index_two];
	free(cwd);
	return (matrix_index);
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
	char	*cwd;
	char	*export[3];

	if (check_invalid_args(arguments, minishell->env_variables))
		return ;
	cwd = getcwd(NULL, 0);
	if (!cwd || (arguments[1] && chdir(arguments[1])) || \
		(!arguments[1] && chdir(arguments[0])))
		return (g_code = 1, perror("cd"), free(cwd));
	ft_memset(export, 0, 3 * sizeof(char *));
	export[1] = make_value("OLDPWD=", cwd, 7 + ft_strlen(cwd), "cd");
	ft_export(minishell, export);
	free(export[1]);
	cwd = getcwd(NULL, 0);
	export[1] = make_value("PWD=", cwd, 4 + ft_strlen(cwd), "cd");
	ft_export(minishell, export);
	free(export[1]);
}
