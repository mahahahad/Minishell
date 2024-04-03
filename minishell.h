#ifndef MINISHELL_H
# define MINISHELL_H

# define PROMPT "minishell> "

# include <readline/readline.h>
# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <sys/wait.h>
# include <stdbool.h>

typedef struct s_environment t_env;

struct s_environment
{
	char	*env_var;
	t_env	*next;
};

#endif
