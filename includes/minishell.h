/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maabdull <maabdull@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 17:24:25 by maabdull          #+#    #+#             */
/*   Updated: 2024/08/13 21:17:35 by maabdull         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define RED "\033[31m"
# define GREEN "\033[32m"
# define YELLOW "\033[33m"
# define BLUE "\033[34m"
# define B_YELLOW "\033[33;1m"
# define RESET "\033[0m"

# ifdef __linux__
#  include <linux/limits.h>
# else
#  include <limits.h>
# endif
# include <dirent.h>
# include <errno.h>
# include <fcntl.h>
# include "libft.h"
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <termios.h>
# include <time.h>

/** GLOBAL VARIABLE **/
extern unsigned int			g_code;

/** STRUCTURES **/
typedef struct s_minishell	t_minishell;
typedef struct s_prompt		t_prompt;
typedef struct stat			t_stat;
typedef struct s_token		t_token;
typedef enum e_token_types	t_tkn_type;
typedef enum e_cmd_types	t_cmd_type;
typedef enum e_sig_rec		t_sig_rec;
typedef enum e_bltn			t_bltn;
typedef struct dirent		t_dir;
typedef struct s_env		t_env;

// Command Types
typedef struct s_cmd		t_cmd;
typedef struct s_cmd_exec	t_cmd_exec;
typedef struct s_cmd_redir	t_cmd_redir;
typedef struct s_cmd_expr	t_cmd_expr;
// typedef struct s_cmd_hdoc	t_cmd_heredoc;

enum e_token_types
{
	LESS,
	DBL_LESS,
	GREAT,
	DBL_GREAT,
	WORD,
	PIPE,
	OR,
	AND,
	P_CLOSE,
	P_OPEN
};

enum e_cmd_types
{
	CMD_EXEC,
	CMD_PIPE,
	CMD_LESS,
	CMD_HEREDOC,
	CMD_GREAT,
	CMD_DBL_GREAT,
	CMD_AND,
	CMD_OR
};

enum e_sig_rec
{
	CHILD,
	PARENT
};

# undef ECHO

enum e_bltn
{
	CD,
	EXIT,
	UNSET,
	ECHO,
	ENV,
	EXPORT,
	PWD,
	NONE
};

struct s_token
{
	t_tkn_type	type;
	char		*content;
	int			id;
	t_token		*next;
};

struct s_env
{
	char		*key;
	char		*value;
	t_env		*next;
};

struct s_minishell
{
	bool		input_fd;
	bool		invalid;
	bool		output_fd;
	bool		piped;
	char		**envp;
	int			envp_count;
	int			pipe_fds[2];
	int			pipe_read_store;
	int			process_id;
	int			token_count;
	t_bltn		bltn;
	t_env		*env_variables;
	t_cmd		*cmd;
	t_cmd		*cmd_head;
	t_token		*tokens;
	t_token		*tokens_head;
};

/*
 * General command structure.
 *
 * Every parse function returns this and can be casted to another cmd type
 * because they all share the type variable
 *
 */
struct s_cmd
{
	t_cmd_type	type;
};

struct s_cmd_exec
{
	t_cmd_type	type;
	t_token		*tokens;
};

struct s_cmd_redir
{
	t_cmd_type	type;
	t_cmd		*cmd;
	char		*file;
	int			fd;
};

struct s_cmd_expr
{
	t_cmd_type	type;
	t_cmd		*command_left;
	t_cmd		*command_right;
};

/** FUNCTIONS **/

// Signals
void	receive_signal(t_sig_rec receiver);

// Parsing
bool	add_token_back(t_token **tokens_list, t_token *token);
t_cmd	*create_expr_cmd(t_cmd_type type, t_cmd *cmd_left, t_cmd *cmd_right);
bool	count_quotations(char *line);
int		count_tokens(char *input);
char	*dollar_expansion(char *token, t_env *list);
t_cmd	*print_syntax_error(char *token, t_cmd *command);
char	*get_token(char **input);
int		get_token_type(char *content);
bool	has_expr_symbol(t_cmd *command);
t_token	*new_token(char *content, int type, t_env *list, bool expand);
void	parse(t_minishell *minishell, char *line, char *store);
t_cmd	*parse_redir(t_cmd *cmd, t_minishell *minishell);
t_token	*token_duplicate(t_token *token);
bool	valid_parenthesis(char *line);
t_token	*wildcard_expansion(char *token, int location, int id, t_token *store);

// Execution
t_bltn	confirm_builtin(char *str);
bool	confirm_command(char **cmd, t_env *environment);
bool	duplicate_fds(t_cmd	*cmd, t_minishell *minishell);
void	execute_builtin(char **cmd, t_minishell *minishell);
void	run_command(t_minishell *minishell);
bool	open_fds(t_cmd *command);

// Built-ins
bool	add_to_matrix(t_minishell *minishell, char *new_var);
void	create_new_variable(t_env *new_var, int *length, char *string);
void	ft_cd(char **cmd, t_minishell *minishell);
void	ft_echo(char **cmd);
void	ft_env(char **args, char **envp);
void	ft_exit(char **arguments, t_minishell *minishell);
void	ft_export(t_minishell *minishell, char **new_variable);
void	ft_pwd(char **args);
void	ft_unset(t_minishell *minishell, char **variable);
bool	is_argument_valid(char *string);
char	*make_value(char *key, char *cwd, int total_length, char *err);

// Cleanup
void	free_char_command(char **cmd);
void	free_command(t_cmd *cmd);
void	free_environment(t_minishell *minishell);
void	free_parsing(t_minishell *minishell);
void	free_tokens(t_token **list);

// Setup
void	setup_environment(t_minishell *minishell, char **env);

// Miscellaneous
void	ft_lstadd_back(t_env **list, t_env *new_node);

# ifndef __linux__

void	rl_replace_line(const char *text, int clear_undo);
int		rl_clear_history(void);

# endif

// Debug
void	print_cmd(t_cmd *cmd, int node_depth, char *prefix);

/**
 * @brief alias for the print_cmd function which allows users to not specify the
 * node_depth while calling it manually.
 */
// # define PRINT_CMD(cmd, ...) print_cmd(cmd, 0, "  ")

#endif
