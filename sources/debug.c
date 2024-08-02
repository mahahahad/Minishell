/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdanish <mdanish@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 12:31:23 by maabdull          #+#    #+#             */
/*   Updated: 2024/08/02 21:53:16 by mdanish          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * This file contains general debugging functions such as printing tokens.
 */

#include "minishell.h"

void	print_tabs(int node_depth, char *prefix)
{
	while (node_depth)
	{
		node_depth--;
		ft_putstr_fd(prefix, 1);
	}
}

void	print_tab_str(char *str, int node_depth, char *prefix)
{
	print_tabs(node_depth, prefix);
	ft_putendl_fd(str, 1);
}

void	print_token(t_token token, int node_depth, char *prefix)
{
	char	*type;

	switch (token.type){
		case PIPE:
			type = "|";
			break ;
		case LESS:
			type = "<";
			break ;
		case DBL_LESS:
			type = "<<";
			break ;
		case GREAT:
			type = ">";
			break ;
		case DBL_GREAT:
			type = ">>";
			break ;
		case OR:
			type = "OR";
			break ;
		case AND:
			type = "AND";
			break ;
		default :
			type = "Word";
	}
	print_tab_str("{", node_depth, prefix);
	print_tabs(node_depth + 1, prefix);
	ft_putstr_fd("Content: ", 1);
	ft_putendl_fd(token.content, 1);
	print_tabs(node_depth + 1, prefix);
	ft_putstr_fd("Type: ", 1);
	ft_putendl_fd(type, 1);
	print_tab_str("}", node_depth, prefix);
}

void	print_tokens(t_token *tokens, int node_depth, char *prefix)
{
	t_token	*temp;

	temp = tokens;
	while (temp)
	{
		print_token(*temp, node_depth, prefix);
		temp = temp->next;
	}
}

void	print_exec_cmd(t_cmd_exec *cmd, int node_depth, char *prefix)
{
	print_tab_str("Executable", node_depth, prefix);
	print_tokens(cmd->tokens, node_depth + 1, prefix);
}

void	print_redir_cmd(t_cmd_redir *cmd, int node_depth, char *prefix)
{
	switch (cmd->type)
	{
		case CMD_GREAT:
			print_tab_str("Output Redirection", node_depth, prefix);
			break;
		case CMD_LESS:
			print_tab_str("Input Redirection", node_depth, prefix);
			break;
		case CMD_DBL_GREAT:
			print_tab_str("Output Redirection (Append)", node_depth, prefix);
			break;
		case CMD_HEREDOC:
			print_tab_str("Heredoc", node_depth, prefix);
			break;
		default:
			break;
	}
	print_tab_str("File", node_depth + 1, prefix);
	print_tab_str(((t_cmd_redir *)cmd)->file, node_depth + 2, prefix);
	print_tab_str("Command", node_depth + 1, prefix);
	print_cmd(((t_cmd_redir *) cmd)->cmd, node_depth + 2, prefix);
}

void	print_expr_cmd(t_cmd_expr *cmd, int node_depth, char *prefix)
{
	switch (cmd->type)
	{
		case CMD_PIPE:
			print_tab_str("Pipe", node_depth, prefix);
			break;
		case CMD_AND:
			print_tab_str("And", node_depth, prefix);
			break;
		case CMD_OR:
			print_tab_str("Or", node_depth, prefix);
			break;
		default:
			break;
	}
	print_tab_str("Left", node_depth + 1, prefix);
	print_cmd(cmd->cmd_left, node_depth + 2, prefix);
	print_tab_str("Right", node_depth + 1, prefix);
	print_cmd(cmd->cmd_right, node_depth + 2, prefix);
}

// void	print_heredoc_cmd(t_cmd_heredoc *cmd, int node_depth, char *prefix)
// {
// 	print_tab_str("Heredoc", node_depth, prefix);
// 	print_tab_str("Delimiter", node_depth + 1, prefix);
// 	print_tab_str(cmd->delimiter, node_depth + 2, prefix);
// 	print_tab_str("Command", node_depth + 1, prefix);
// 	print_cmd(cmd->cmd, node_depth + 2, prefix);
// }

/**
 * @brief Command struct printing function. Useful for debugging the 
 * completely parsed command tree.
 * 
 * @param cmd the parsed command tree
 * @param node_depth the level of indentation to use before printing.
 * (0 for normal calls)
 * @param prefix the string to prefix each line with
 */
void	print_cmd(t_cmd *cmd, int node_depth, char *prefix)
{
	if (!cmd)
		return ;
	switch (cmd->type)
	{
		case CMD_EXEC:
		 	print_exec_cmd((t_cmd_exec *) cmd, node_depth, prefix);
			break;

		case CMD_DBL_GREAT:
			__attribute__ ((fallthrough));
		case CMD_LESS:
		case CMD_GREAT:
		case CMD_HEREDOC:
			print_redir_cmd((t_cmd_redir *) cmd, node_depth, prefix);
			break;

		// case CMD_HEREDOC:
		// 	print_heredoc_cmd((t_cmd_heredoc *) cmd, node_depth, prefix);
		// 	break;

		case CMD_AND:
			__attribute__((fallthrough));
		case CMD_OR:
		case CMD_PIPE:
			print_expr_cmd((t_cmd_expr *) cmd, node_depth, prefix);
			break;
	}
}
