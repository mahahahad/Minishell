/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdanish <mdanish@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 17:38:52 by maabdull          #+#    #+#             */
/*   Updated: 2024/07/28 15:58:32 by mdanish          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Frees up the token list.
 * 
 * Simply goes through the list and frees the content and the node itself.
 * It can be used in case of malloc fail or when performing a general cleanup.
 * 
 * @param list is the list of tokens.
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
 * @param cmd is the list of the command.
 */
void	free_cmd(t_cmd *cmd)
{
	if (!cmd)
		return ;
	if (cmd->type == CMD_PIPE || cmd->type == CMD_AND || cmd->type == CMD_OR)
	{
		free_cmd(((t_cmd_expr *)cmd)->cmd_left);
		free_cmd(((t_cmd_expr *)cmd)->cmd_right);
	}
	else if (cmd->type == CMD_GREAT || cmd->type == CMD_DBL_GREAT || \
		cmd->type == CMD_LESS || cmd->type == CMD_HEREDOC)
	{
		if (((t_cmd_redir *)cmd)->fd > -1)
			close(((t_cmd_redir *)cmd)->fd);
		free_cmd(((t_cmd_redir *)cmd)->cmd);
	}
	else if (cmd->type == CMD_EXEC)
		free_tokens(&((t_cmd_exec *)cmd)->tokens);
	free(cmd);
}

/**
 * @brief Frees the char double pointer version of the command.
 * 
 * @param cmd is the double pointer of the command.
 */
void	free_char_cmd(char **cmd, char *original)
{
	int	cmd_count;

	cmd_count = -1;
	while (cmd[++cmd_count])
		free(cmd[cmd_count]);
	free(cmd[cmd_count]);
	free(cmd);
	free(original);
}

/**
 * @brief General command to free up the entirety of parsing
 * 
 * Calls the free_tokens() and the free_cmd() functions to cleanup the command
 * and the token list. It also sets the pointer to tokens to NULL;
 * 
 * @param cmd is the double pointer of the command.
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
