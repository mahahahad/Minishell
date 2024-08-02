/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdanish <mdanish@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 17:38:52 by maabdull          #+#    #+#             */
/*   Updated: 2024/08/02 21:56:02 by mdanish          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Frees up the token list.
 * 
 * Simply goes through the list and frees the content and the node itself.
 * It can be used in case of malloc fail or when performing a general cleanup.
 * 
 * @param tokens is the list of tokens.
 */
void	free_tokens(t_token **tokens)
{
	t_token	*current;
	t_token	*next;

	current = *tokens;
	while (current)
	{
		next = current->next;
		free(current->content);
		free(current);
		current = next;
	}
	*tokens = NULL;
}

/**
 * @brief Frees the linked list version of the command.
 * 
 * @param command is the list of the command.
 */
void	free_cmd(t_cmd *command)
{
	if (!command)
		return ;
	if (command->type == CMD_PIPE || command->type == CMD_AND || \
		command->type == CMD_OR)
	{
		free_cmd(((t_cmd_expr *)command)->cmd_left);
		free_cmd(((t_cmd_expr *)command)->cmd_right);
	}
	else if (command->type == CMD_GREAT || command->type == CMD_DBL_GREAT || \
		command->type == CMD_LESS || command->type == CMD_HEREDOC)
	{
		if (((t_cmd_redir *)command)->fd > -1)
			close(((t_cmd_redir *)command)->fd);
		free_cmd(((t_cmd_redir *)command)->cmd);
	}
	else if (command->type == CMD_EXEC)
		free_tokens(&((t_cmd_exec *)command)->tokens);
	free(command);
}

/**
 * @brief Frees the char double pointer version of the command.
 * 
 * @param command is the double pointer of the command.
 */
void	free_char_command(char **command)
{
	int	cmd_count;

	cmd_count = -1;
	while (command[++cmd_count])
		free(command[cmd_count]);
	free(command[cmd_count]);
	free(command);
}

/**
 * @brief General command to free up the entirety of parsing
 * 
 * Calls the free_tokens() and the free_cmd() functions to cleanup the command
 * and the token list. It also sets the pointer to tokens to NULL;
 * 
 * @param minishell contains the stuff to be freed.
 */
void	free_parsing(t_minishell *minishell)
{
	free_cmd(minishell->cmd);
	free_tokens(&minishell->tokens_head);
	minishell->tokens = NULL;
}

/**
 * @brief Cleanup of the environment in case of any malloc fail.
 * 
 * The function calls free_split() on the char double pointer version of the
 * environment variables. The list version of it is freed node by node along
 * with the contents of it. Both versions are set to NULL after freeing.
 * 
 * @param minishell contains the environment that needs to be freed.
 */
void	free_environment(t_minishell *minishell)
{
	t_env	*store;

	free_split(minishell->envp, minishell->envp_count);
	minishell->envp = NULL;
	while (minishell->env_variables)
	{
		store = minishell->env_variables->next;
		free(minishell->env_variables->key);
		free(minishell->env_variables->value);
		free(minishell->env_variables);
		minishell->env_variables = store;
	}
	minishell->env_variables = NULL;
}
