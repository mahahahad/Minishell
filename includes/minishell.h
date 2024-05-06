/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maabdull <maabdull@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 17:24:25 by maabdull          #+#    #+#             */
/*   Updated: 2024/05/06 19:15:16 by maabdull         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define PROMPT "minishell> "
# define RED "\033[31m"
# define GREEN "\033[32m"
# define YELLOW "\033[33m"
# define BLUE "\033[34m"
# define B_YELLOW "\033[33;1m"
# define RESET "\033[0m"

# include "libft.h"
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/wait.h>
# include <termios.h>
# include <time.h>
# include <unistd.h>

/** STRUCTURES **/
enum		e_token_types
{
	PIPE,
	LESS,
	DBL_LESS,
	GREAT,
	DBL_GREAT,
	WORD,
	ERR
};

enum		e_cmd_types
{
	CMD_EXEC
};

typedef struct s_token
{
	int		type;
	char	*content;
}			t_token;

typedef struct s_prompt
{
	char	*previous;
	char	*current;
} t_prompt;

typedef struct s_minishell
{
	t_token	*tokens;
	int		token_count;
	t_prompt	*prompt;
}			t_minishell;

/*
 * General command structure.
 *
 * Every parse function returns this and can be casted to another cmd type
 * because they all share the type variable
 * */
typedef struct s_cmd
{
	int		type;
}			t_cmd;

typedef struct s_cmd_exec
{
	int		type;
	char	**tokens;
}			t_cmd_exec;

/** GLOBAL VARIABLE **/
extern int	g_status_code;

/** FUNCTIONS **/
// Parsing
void		parse(t_minishell *minishell, char *line);
t_cmd		*create_exec_cmd(t_minishell *minishell);

// Execution
int			exec_cmd(char **cmd, char **env);
void		exec_builtin(char **cmd);
void		run_cmd(t_cmd *cmd, char **env);

// Built-ins
bool		is_builtin(char *str);
void		echo(char **cmd);
int			cd(char **cmd);

// Cleanup
void		free_tokens(t_minishell *minishell);
void		free_cmd(t_cmd *cmd);

// General
t_prompt	*init_prompt(void);
void	update_prompt(t_prompt *prompt);

#include <readline/readline.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

#endif
