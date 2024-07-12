/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdanish <mdanish@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 14:41:15 by maabdull          #+#    #+#             */
/*   Updated: 2024/07/12 18:28:17 by mdanish          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * This file contains functions directly used for parsing commands
 */

/**
 * @brief Parse the command line.
 * 
 * This function is the main parsing function that is called on every
 * line after receiving input. It performs the pre-requisite checks and
 * makes the command line accessible to all the subsequent parsing functions
 * by splitting them into tokens. It then starts the parsing process by calling
 * the parse_expr function which recursively builds the command tree.
 * 
 * Performs some checks prior to parsing incase of an early return due to
 * incorrect input. Such as:
 * - Checking if the line contains anything, which is used for detecting 
 * no-command enter key presses.
 * - Checking if the number of quotations are uneven, which is used for
 * detecting open quotes.
 * Then, splits the command line into tokens wherever appropriate and sets them
 * in the minishell struct to use in other functions.
 * Finally, calls the parse_expr function which adds to the command tree
 * following the proper grammar.
 * 
 * @param minishell the common minishell struct
 * @param line the raw user input line
 * @return t_cmd* the parsed command tree 
 */
t_cmd	*parse(t_minishell *minishell, char *line)
{
	int		i;

	if (!line || !line[0] || count_quotations(line) || !valid_brackets(line))
		return (NULL);
	minishell->token_count = count_tokens(line);
	//! For debugging purposes
	// ft_putstr_fd("There are ", 1);
	// ft_putnbr_fd(minishell->token_count, 1);
	// ft_putendl_fd(" tokens in your input", 1);
	i = -1;
	while (++i < minishell->token_count)
	{
		add_token_back(&minishell->tokens, \
			new_token(get_token(&line), minishell->env_variables));
		// if (!minishell->tokens) ------> malloc fail handling required!!!
	}
	minishell->tokens_head = minishell->tokens;
	return (parse_expr(minishell));
}

/**
 * @brief Parse a general expression.
 * 
 * Expressions are commands that follow the format:
 * <CMD_LEFT> <SYMBOL> <CMD_RIGHT>
 * This function is called from the main parse function as it calls the 
 * parse_exec function first and then checks if there are remaining tokens 
 * which would mean that a delimiting symbol was found and the command structure
 * that it retured is a valid command that can be used as the left command of
 * the expression.
 * This ensures that if the command line contains expressions, they will have
 * a valid command to their right and left. But if they do not contain
 * expressions, the command line will still contain a valid command.
 * 
 * Calls the parse_exec function which creates a command structure ensuring
 * that there are valid commands surrounding the symbol in expression
 * (|, ||, &&) commands.
 * Checks and returns if the command structure is null, which is used 
 * for identifying errors.
 * Checks if there are any remaining tokens yet to be parsed. Returns the
 * command if there aren't, since that means the command line has been fully 
 * parsed. Continues if there are, which means that a token containing a symbol 
 * that separates expressions was found.
 * Checks the type of this token and creates the appropriate struct by first
 * parsing another expression command by calling parse_expr again on the
 * remaining tokens which ensures a valid right command.
 * Returns this command structure.
 * 
 * @param minishell
 * @return t_cmd*
 */
t_cmd	*parse_expr(t_minishell *minishell)
{
	t_cmd	*cmd;

	cmd = parse_exec(minishell);
	if (!cmd)
		return (NULL);
	if (!minishell->tokens)
		return (cmd);
	if (minishell->tokens->type == PIPE)
	{
		minishell->tokens = minishell->tokens->next;
		cmd = create_expr_cmd(CMD_PIPE, cmd, parse_expr(minishell));
	}
	else if (minishell->tokens->type == AND)
	{
		minishell->tokens = minishell->tokens->next;
		cmd = create_expr_cmd(CMD_AND, cmd, parse_expr(minishell));
	}
	else if (minishell->tokens->type == OR)
	{
		minishell->tokens = minishell->tokens->next;
		cmd = create_expr_cmd(CMD_OR, cmd, parse_expr(minishell));
	}
	return (cmd);
}

/**
 * @brief Parse an executable command.
 *
 * This function is called to create an executable command with the tokens, 
 * starting from the active token, till a forbidden token is found.
 * Executable commands follow the format:
 * [<REDIRECTION>] <WORD> [<COMMAND>]
 * This means that they can start with only an optional redirection symbol (<, 
 * <<, >, >>) or a word.
 * Executable commands are the most common type of commands as every other
 * command type contains them in one way or another.
 * If an executable command starts with a redirection, this function creates
 * a redirection command and sets its command to be the executable command
 * which it then updates through the loop.
 * 
 * Creates an empty command structure which will be filled with tokens
 * Checks if the starting token is a redirection token and parses accordingly
 * by calling the parse_redir function.
 * Starts a loop until all the tokens have finished and performs the following
 * actions:
 * - Checks if the current token is of a type that should break the loop
 * (|, ||, &&)
 * - Checks if there are no tokens in the command struct (implying this is the 
 * first one) and prints the appropriate error message (because commands can
 * not start with these token types)
 * - Since the current token is valid, pushes it to the command struct.
 * - Move the current token to the next one.
 * - Check and parse incase a redirection token type was found using 
 * parse_redir.
 * Returns the command structure once it is finished.
 * 
 * @param minishell
 * @return t_cmd*
 */
t_cmd	*parse_exec(t_minishell *minishell)
{
	t_cmd		*node;
	t_cmd_exec	*cmd;
	t_token		*current_token;

	node = ft_calloc(1, sizeof(t_cmd_exec));
	cmd = (t_cmd_exec *)node;
	while (minishell->tokens)
	{
		node = parse_redir(node, minishell);
		if (!node || !minishell->tokens)
			return (node);
		if (is_exec_delimiter(minishell))
		{
			if (!cmd->tokens)
				return (print_exec_parse_err(minishell));
			break ;
		}
		current_token = minishell->tokens;
		minishell->tokens = minishell->tokens->next;
		current_token->next = NULL;
		add_token_back(&cmd->tokens, current_token);
	}
	return (node);
}

/**
 * @brief Parse a redirection command.
 * 
 * This function ensures the current token and next token match the format
 * needed for redirection commands:
 * <SYMBOL> <FILE_NAME>
 * The command which is to be redirected is provided to the function as an
 * argument.

 * Starts a loop until either the tokens are over, or the current token's type
 * is a non-redirection type and performs the following actions:
 * - Check and return if the next token doesn't exist or isn't of the WORD type
 * - Check and create a redirection command struct based on the type of 
 * redirection
 * Return the original or modified command struct
 * 
 * @param cmd
 * @param minishell
 * @return t_cmd*
 */
t_cmd	*parse_redir(t_cmd *cmd, t_minishell *minishell)
{
	char	*content;

	while (minishell->tokens && (minishell->tokens->type == GREAT || \
		minishell->tokens->type == LESS || \
		minishell->tokens->type == DBL_GREAT || \
		minishell->tokens->type == DBL_LESS))
	{
		content = minishell->tokens->next->content;
		if (!minishell->tokens->next || minishell->tokens->next->type != WORD)
			return (ft_putendl_fd("No file for redirection found", 1), NULL);
		if (minishell->tokens->type == LESS)
			cmd = create_redir_cmd(cmd, CMD_LESS, content);
		else if (minishell->tokens->type == GREAT)
			cmd = create_redir_cmd(cmd, CMD_GREAT, content);
		else if (minishell->tokens->type == DBL_GREAT)
			cmd = create_redir_cmd(cmd, CMD_DBL_GREAT, content);
		else if (minishell->tokens->type == DBL_LESS)
			cmd = create_heredoc(cmd, content);
		minishell->tokens = minishell->tokens->next->next;
	}
	return (cmd);
}
