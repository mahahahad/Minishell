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

enum	e_token_types
{
	PIPE,
	LESS,
	DBL_LESS,
	GREAT,
	DBL_GREAT,
	WORD
};

typedef struct s_token
{
	int	type;
	char	*content;
} t_token;

typedef struct s_minishell
{
	t_token	*tokens;
}			t_minishell;

typedef struct s_cmd
{
} t_cmd;

#endif
