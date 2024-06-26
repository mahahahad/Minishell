/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdanish <mdanish@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 17:24:25 by maabdull          #+#    #+#             */
/*   Updated: 2024/06/20 23:37:30 by mdanish          ###   ########.fr       */
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
# include "libft.h"
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
typedef enum e_token_types	t_type;
typedef struct dirent		t_dir;
typedef struct s_cmd		t_cmd;
typedef struct s_cmd_exec	t_cmd_exec;
typedef struct s_env		t_env;
typedef struct s_minishell	t_minishell;
typedef struct s_token		t_token;

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
	CMD_EXEC
};

struct s_token
{
	char	*content;
	t_type	type;
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
	int		type;
};

struct s_cmd_exec
{
	char	**tokens;
	int		type;
};

/** FUNCTIONS **/
// Parsing
char		*dollar_expansion(char *token, t_env *list);
t_cmd		*create_exec_cmd(t_minishell *minishell);
void		parse(t_minishell *minishell, char *line);
char		*wildcards(char *token, char *store);

// Execution
void		exec_builtin(char **cmd, t_minishell *minishell);
int			exec_cmd(char **cmd, char **env);
bool		is_builtin(char *str);
void		run_cmd(t_cmd *cmd, char **env, t_minishell *minishell);

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
void		ft_lstadd_back(t_env **list, t_env *new_node);

#endif
