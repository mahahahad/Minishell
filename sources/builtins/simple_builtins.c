/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simple_builtins.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdanish <mdanish@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 13:21:03 by mdanish           #+#    #+#             */
/*   Updated: 2024/07/21 12:55:25 by mdanish          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Prints to standard output a line of text.
 * 
 * It prints the arguments till cmd reaches null. While printing, it ensures
 * there is an argument after the current one before it prints a space to act
 * as a delimiter between the arguments.
 * 
 * It then confirms that the first argument is not the [ -n ] flag and prints
 * the new line after it.
 * 
 * After completion, it sets the g_status_code to 0.
 * 
 * @param cmd points to the strings ft_echo needs to print. (NULL terminated)
 *
 */
void	ft_echo(char **cmd)
{
	int		index;
	int		i;
	bool	option_flag;

	index = 0;
	option_flag = false;
	while (cmd[++index] && cmd[index][0] == '-')
	{
		i = 1;
		while (cmd[index][i] == 'n')
			i++;
		if (!cmd[index][i])
			option_flag = true;
		else
			break ;
	}
	while (cmd[index])
	{
		ft_putstr_fd(cmd[index], 1);
		if (cmd[++index])
			ft_putchar_fd(' ', 1);
	}
	if (!option_flag)
		ft_putchar_fd('\n', 1);
	g_status_code = 0;
}

/**
 * @brief Prints to standard output the environment variables.
 * 
 * It goes through the matrix of the environment variables and sends them to
 * ft_putendl_fd() to print to stdout.
 * 
 * Options or arguments will result in an error message and the g_status_code
 * being set to 1.
 * 
 * Upon completion, it sets the g_status_code to 0.
 * 
 * @param args a char ** pointing to the arguments sent by the command line.
 * @param envp a char ** pointing to the matrix ft_env needs to print.
 *
 */
void	ft_env(char **args, char **envp)
{
	if (args[1] && args[1][0])
	{
		g_status_code = 1;
		return (ft_putendl_fd("env does not accecpt options or arguments.", 2));
	}
	while (*envp)
	{
		ft_putendl_fd(*envp, 1);
		envp++;
	}
	g_status_code = 0;
}

/**
 * @brief Prints to standard output the current working directory.
 * 
 * Gets the current working directory through a call to the getcwd() function
 * and prints it to the stdout using ft_putendl_fd().
 * 
 * Options or arguments will result in an error message and
 * the g_status_code being set to 1.
 * 
 * Upon completion, it sets the g_status_code to 0.
 * 
 * @param args is used to ensure that no arguments or options are provided.
 *
 */
void	ft_pwd(char **args)
{
	char	current_working_directory[PATH_MAX];

	if (args[1] && args[1][0] == '-')
	{
		g_status_code = 1;
		return (ft_putendl_fd("pwd does not accecpt options.", 2));
	}
	current_working_directory[0] = '\0';
	getcwd(current_working_directory, PATH_MAX);
	ft_putendl_fd(current_working_directory, 1);
	g_status_code = 0;
}
