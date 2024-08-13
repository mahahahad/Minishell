/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maabdull <maabdull@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 14:41:15 by maabdull          #+#    #+#             */
/*   Updated: 2024/08/13 21:25:38 by maabdull         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * This file contains functions directly used for parsing commands
 */

static t_cmd	*parse_execution(t_minishell *minishell);
static t_cmd	*parse_expression(t_cmd *command_left, t_minishell *minishell);

/**
 * @brief Parse a logical expression command.
 *
 * This function ensures the correct format is followed for logical expressions.
 * <C_LEFT> <OPERATOR> <C_RIGHT>
 *
 * @param command_left The command to be used as the command_left for the
 * expression struct. It is needed here to parse logical commands in brackets.
 * @param minishell
 *
 * @return the command tree with the boolean operators parsed.
 */
static t_cmd	*parse_logical_expr(t_cmd *command_left, t_minishell *minishell)
{
	t_cmd	*cmd;

	if (!command_left)
		cmd = parse_execution(minishell);
	else
		cmd = command_left;
	if (cmd && minishell->tokens->type == AND)
	{
		minishell->tokens = minishell->tokens->next;
		if (!minishell->tokens || minishell->tokens->type == P_CLOSE)
			return (print_syntax_error("&&", cmd));
		cmd = create_expr_cmd(CMD_AND, cmd, parse_expression(NULL, minishell));
	}
	else if (cmd && minishell->tokens->type == OR)
	{
		minishell->tokens = minishell->tokens->next;
		if (!minishell->tokens || minishell->tokens->type == P_CLOSE)
			return (print_syntax_error("||", cmd));
		cmd = create_expr_cmd(CMD_OR, cmd, parse_execution(minishell));
		if (minishell->tokens)
			cmd = parse_expression(cmd, minishell);
	}
	else if (cmd && !has_expr_symbol(cmd))
		return (print_syntax_error(minishell->tokens->content, cmd));
	return (cmd);
}

/**
 * @brief Parse a pair of parenthesis.
 *
 * This function ensures that commands that are within brackets
 * follow the proper format:
 * <PARAN_OPEN> <C_LEFT> <OPERATOR> <C_RIGHT> <PARAN_CLOSE>
 * It also throws an error if an operator is not detected after the first
 * command.
 *
 * Checks if the first token is an opening parenthesis.
 * Moves the tokens list to the next token if it is and performs various checks.
 * Calls the parse_logical_expr function that checks if the tokens are in the
 * form needed for logical expressions. This function returns NULL when a non
 * logical token type is found.
 * Moves the tokens list to the next token to skip the closing parenthesis.
 *
 *
 * @param command The created command structure that will hold the executable
 * @param minishell The minishell datastruct
 *
 * @return The parsed logical command tree
 */
static t_cmd	*parse_parenthesis(t_minishell *minishell)
{
	t_cmd	*command;

	minishell->tokens = minishell->tokens->next;
	command = parse_logical_expr(NULL, minishell);
	if (command && minishell->tokens)
		minishell->tokens = minishell->tokens->next;
	return (command);
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
 * Calls the parse_paranthesis function on the very first token to ensure it is
 * not an opening paranthesis as this is the only place where this is valid.
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
 *
 * @return the command tree with the command parsed.
 */
static t_cmd	*parse_execution(t_minishell *minishell)
{
	t_cmd		*node;
	t_cmd_exec	*command;

	if (minishell->tokens->type == P_OPEN)
		return (parse_parenthesis(minishell));
	node = ft_calloc(1, sizeof(t_cmd_exec));
	if (!node)
		return (perror("Tokenisation"), g_code = 1, NULL);
	command = (t_cmd_exec *)node;
	while (minishell->tokens)
	{
		node = parse_redir(node, minishell);
		if (!node || !minishell->tokens)
			return (node);
		if (minishell->tokens->type == P_OPEN || (!command->tokens && \
			minishell->tokens->type > WORD && minishell->tokens->type < P_OPEN))
			return (print_syntax_error(minishell->tokens->content, node));
		if (minishell->tokens->type > WORD && minishell->tokens->type < P_OPEN)
			break ;
		add_token_back(&command->tokens, token_duplicate(minishell->tokens));
		if (!command->tokens)
			return (free_command(node), NULL);
		minishell->tokens = minishell->tokens->next;
	}
	return (node);
}

/**
 * @brief Parse a general expression.
 *
 * Expressions are commands that follow the format:
 * <C_LEFT> <SYMBOL> <C_RIGHT>
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
 * @param command_left The command to be used as the command_left for the
 * expression struct. This is necessary for properly parsing the OR operator
 * (see the parse_logical_expr function)
 * @param minishell
 *
 * @return The command tree with the AND, OR and PIPE parsed.
 */
static t_cmd	*parse_expression(t_cmd *command_left, t_minishell *minishell)
{
	t_cmd	*command;

	if (command_left)
		command = command_left;
	else
		command = parse_execution(minishell);
	if (!command || !minishell->tokens)
		return (command);
	if (minishell->tokens->type == PIPE)
	{
		if (!minishell->tokens->next)
			return (print_syntax_error("|", command));
		minishell->tokens = minishell->tokens->next;
		command = create_expr_cmd(CMD_PIPE, command,
				parse_expression(NULL, minishell));
	}
	else if (minishell->tokens->type == AND || minishell->tokens->type == OR)
		command = parse_logical_expr(command, minishell);
	return (command);
}

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
void	parse(t_minishell *minishell, char *line, char *store)
{
	char	*token;
	int		token_count;

	if (!line || !line[0] || count_quotations(line) || !valid_parenthesis(line))
		return ;
	minishell->pipe_fds[0] = -1;
	minishell->pipe_fds[1] = -1;
	minishell->pipe_read_store = -1;
	minishell->piped = false;
	minishell->token_count = count_tokens(line);
	token_count = -1;
	while (++token_count < minishell->token_count)
	{
		token = get_token(&line);
		add_token_back(&minishell->tokens, \
		new_token(token, get_token_type(token), minishell->env_variables, 1));
	}
	minishell->tokens_head = minishell->tokens;
	if (!minishell->tokens_head)
		return (free(store));
	minishell->cmd = parse_expression(NULL, minishell);
	minishell->cmd_head = minishell->cmd;
	if (minishell->token_count)
		add_history(store);
	free(store);
}
