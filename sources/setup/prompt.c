/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdanish <mdanish@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 17:28:38 by maabdull          #+#    #+#             */
/*   Updated: 2024/05/23 23:41:10 by maabdull         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Allocate space for a prompt structure
 */
t_prompt	*init_prompt(void)
{
	t_prompt	*prompt;

	prompt = malloc(sizeof(t_prompt));
	prompt->previous = NULL;
	prompt->current = NULL;
	update_prompt(prompt);
	return (prompt);
}

/**
 * @brief Update the provided prompt with the users current location
 * If current directory doesn't exist, return the previous prompt
 */
void	update_prompt(t_prompt *prompt)
{
	int		i;
	char	*temp;
	char	*prompt_str;
	char	*home_dir;
	char	*current_dir;

	prompt_str = YELLOW "msh" RESET "[" B_YELLOW;
	current_dir = getcwd(NULL, 0);
	home_dir = getenv("HOME");
	i = 0;
	if (!current_dir)
		return ;
	if (home_dir && current_dir)
	{
		i = ft_strlen(home_dir);
		if (ft_strncmp(current_dir, home_dir, i) == 0)
		{
			prompt_str = ft_strjoin(prompt_str, "~");
			temp = prompt_str;
			prompt_str = ft_strjoin(prompt_str, current_dir + i);
			free(temp);
		}
		else
			prompt_str = ft_strjoin(prompt_str, current_dir + i);
	}
	else
		prompt_str = ft_strjoin(prompt_str, current_dir + i);
	temp = prompt_str;
	prompt_str = ft_strjoin(prompt_str, RESET "] ");
	free(temp);
	free(current_dir);
	free(prompt->previous);
	prompt->previous = prompt->current;
	prompt->current = prompt_str;
	return ;
}
