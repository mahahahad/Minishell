/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maabdull <maabdull@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 17:24:25 by maabdull          #+#    #+#             */
/*   Updated: 2024/07/04 13:43:48 by maabdull         ###   ########.fr       */
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

# include <dirent.h>
# include <errno.h>
# include <fcntl.h>
# include "libft.h"
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <string.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <termios.h>
# include <time.h>

/** GLOBAL VARIABLE **/
extern int					g_status_code;

/** STRUCTURES **/
typedef struct s_minishell	t_minishell;
typedef struct s_prompt 	t_prompt;
typedef struct s_token		t_token;
typedef enum e_token_types	t_token_type;
typedef enum e_cmd_types	t_cmd_type;
typedef struct dirent		t_dir;
typedef struct s_env		t_env;

// Command Types
typedef struct s_cmd		t_cmd;
typedef struct s_cmd_exec	t_cmd_exec;
typedef struct s_cmd_redir	t_cmd_redir;
typedef struct s_cmd_expr	t_cmd_expr;

enum e_token_types
{
	PIPE,
	LESS,
	DBL_LESS,
	GREAT,
	DBL_GREAT,
	OR,
	AND,
	WORD,
	ERR
};

enum e_cmd_types
{
	CMD_EXEC,
	CMD_PIPE,
	CMD_LESS,
	CMD_GREAT,
	CMD_DBL_GREAT,
	CMD_AND,
	CMD_OR,
};

struct s_token
{
	t_token_type	type;
	char			*content;
	t_token			*next;
};

struct s_env
{
	char	*key;
	char	*value;
	t_env	*next;
};

struct s_minishell
{
	char	**envp;
	int		token_count;
	int		envp_count;
	t_env	*env_variables;
	t_token	*tokens;
	t_token	*tokens_head; //! can be removed
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
	t_token	*tokens;
};

struct s_cmd_redir
{
	t_cmd_type	type;
	t_cmd	*cmd;
	char	*file;
	int		fd;
};

struct s_cmd_pipe
{
	t_cmd_type	type;
	t_cmd	*cmd_left;
	t_cmd	*cmd_right;
};

struct s_cmd_expr
{
	t_cmd_type	type;
	t_cmd	*cmd_left;
	t_cmd	*cmd_right;
};

/** FUNCTIONS **/
// Parsing
t_cmd			*create_exec_cmd(void);
t_cmd			*create_redir_cmd(t_cmd *cmd, int type, char *file);
t_cmd			*create_expr_cmd(int type, t_cmd *cmd_left, t_cmd *cmd_right);
void			push_token(t_token **tokens_list, t_token *token);
void			tokenize(t_minishell *minishell, char *input);
int				count_quotations(char *line);
t_cmd			*parse(t_minishell *minishell, char *line);
t_cmd			*parse_expr(t_minishell *minishell);
t_cmd			*parse_exec(t_minishell *minishell);
t_cmd			*parse_redir(t_cmd *cmd, t_minishell *minishell);
char		*dollar_expansion(char *token, t_env *list);
char		*wildcards(char *token, char *store);

// Execution
void		exec_builtin(char **cmd, t_minishell *minishell);
void		exec_cmd(char **cmd, char **env);
bool		is_builtin(char *str);
void		run_cmd(t_cmd *cmd, char **env);

// Built-ins
void		add_to_matrix(t_minishell *minishell, char *new_var);
void		create_new_variable(t_env *new_var, int *length, char *string);
void		ft_cd(char **cmd, t_minishell *minishell);
void		ft_echo(char **cmd);
void		ft_env(char **args, char **envp);
void		ft_export(t_minishell *minishell, char **new_variable);
void		ft_pwd(char **args);
void		ft_unset(t_minishell *minishell, char **variable);
bool		is_argument_valid(const char *string);

// Cleanup
void		free_cmd(t_cmd *cmd);
void		free_tokens(t_minishell *minishell);

// Setup
void		setup_environment(t_minishell *minishell, char **env);

// Miscellaneous
void			ft_lstadd_back(t_env **list, t_env *new_node);
void			rl_replace_line(const char *text, int clear_undo);

// Debug
void			print_token(t_token token);

#endif
