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
# include <sys/wait.h>
# include <termios.h>
# include <unistd.h>

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

enum	e_cmd_types
{
	CMD_EXEC
};

typedef struct s_token
{
	int		type;
	char	*content;
}			t_token;

typedef struct s_minishell
{
	t_token	*tokens;
	int	token_count;
}			t_minishell;

/*
 * General command structure.
 *
 * Every parse function returns this and can be casted to another cmd type
 * because they all share the type variable
 * */
typedef struct s_cmd
{
	int	type;
}	t_cmd;

typedef struct s_cmd_exec
{
	int	type;
	char	**tokens;
}			t_cmd_exec;

#endif
