/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simple_builtins.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdanish <mdanish@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 13:21:03 by mdanish           #+#    #+#             */
/*   Updated: 2024/08/08 12:38:17 by mdanish          ###   ########.fr       */
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
 * After completion, it sets the g_code to 0.
 * 
 * @param command points to the strings ft_echo needs to print.
 *
 */
void	ft_echo(char **command)
{
	int		index;
	int		i;
	bool	option_flag;

	index = 0;
	option_flag = false;
	while (command[++index] && command[index][0] == '-')
	{
		i = 1;
		while (command[index][i] == 'n')
			i++;
		if (!command[index][i])
			option_flag = true;
		else
			break ;
	}
	while (command[index])
	{
		ft_putstr_fd(command[index], 1);
		if (command[++index])
			ft_putchar_fd(' ', 1);
	}
	if (!option_flag)
		ft_putchar_fd('\n', 1);
	g_code = 0;
}

/**
 * @brief Prints to standard output the environment variables.
 * 
 * It goes through the matrix of the environment variables and sends them to
 * ft_putendl_fd() to print to stdout.
 * 
 * Options or arguments will result in an error message and the g_code
 * being set to 1.
 * 
 * Upon completion, it sets the g_code to 0.
 * 
 * @param arguments points to the arguments sent by the command line.
 * @param envp points to the matrix ft_env needs to print.
 *
 */
void	ft_env(char **arguments, char **envp)
{
	if (arguments[1] && arguments[1][0])
	{
		g_code = 1;
		return (ft_putendl_fd("env does not accecpt options or arguments.", 2));
	}
	while (*envp)
	{
		ft_putendl_fd(*envp, 1);
		envp++;
	}
	g_code = 0;
}

/**
 * @brief Prints to standard output the current working directory.
 * 
 * Gets the current working directory through a call to the getcwd() function
 * and prints it to the stdout using ft_putendl_fd().
 * 
 * Options or arguments will result in an error message and
 * the g_code being set to 1.
 * 
 * Upon completion, it sets the g_code to 0.
 * 
 * @param arguments is used to ensure that no arguments or options are provided.
 *
 */
void	ft_pwd(char **arguments)
{
	char	*current_working_directory;

	if (arguments[1] && arguments[1][0] == '-')
		return (g_code = 1, ft_putendl_fd("pwd does not accecpt options.", 2));
	current_working_directory = getcwd(NULL, 0);
	if (!current_working_directory)
		return (g_code = WEXITSTATUS(errno), perror("pwd"));
	ft_putendl_fd(current_working_directory, 1);
	g_code = 0;
}

static int	convert_to_exit_code(char *string, int index, int sign)
{
	size_t	number;

	number = 0;
	while (string[index] == 32 || (string[index] > 8 && string[index] < 14))
		index++;
	if (string[index] == '+')
		index++;
	else if (string[index] == '-' && ++index)
		sign = -1;
	while (string[index] == 48)
		index++;
	while (string[index] > 47 && string[index] < 58 && number <= INT64_MAX)
		number = (number * 10) + string[index++] - 48;
	if ((number > INT64_MAX && (sign == 1 || number - INT64_MAX != 1))
		|| string[index])
	{
		ft_putstr_fd("exit: ", 2);
		ft_putstr_fd(string, 2);
		ft_putendl_fd(": numeric argument required", 2);
		return (500);
	}
	return ((number % 256) * sign);
}

void	ft_exit(char **arguments, t_minishell *minishell)
{
	if (arguments[1])
	{
		g_code = convert_to_exit_code(arguments[1], 0, 1);
		if (g_code == 500)
			g_code = 2;
		else if (arguments[2])
			return (g_code = 1, ft_putendl_fd("exit: too many arguments", 2));
	}
	free_char_command(arguments);
	free_parsing(minishell);
	free_environment(minishell);
	exit(g_code);
}
