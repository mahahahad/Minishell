/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_exec.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdanish <mdanish@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 12:05:36 by mdanish           #+#    #+#             */
/*   Updated: 2024/07/30 21:31:15 by mdanish          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Duplicates the fds as necessary.
 * 
 * The function goes through the command tree and identifies the first instance
 * of a redirection and duplicates it. It duplicates to the STDOUT or STDIN on
 * the basis of the command type. While it goes through the tree of the
 * redirections, it closes every single open fd to prevent fd leaks.
 * 
 * @param cmd is the tree to identify redirection from.
 * @param minishell contains flags for the duplication.
 * @param read is a flag to indicate which end of the pipe to duplicate.
 */
void	duplicate_fds(t_cmd	*cmd, t_minishell *minishell, int read)
{
	while (cmd->type > CMD_PIPE && cmd->type < CMD_AND)
	{
		if ((cmd->type == CMD_GREAT || cmd->type == CMD_DBL_GREAT) && \
			!minishell->output_fd && dup2(((t_cmd_redir *)cmd)->fd, 1))
			minishell->output_fd = true;
		else if ((cmd->type == CMD_LESS || cmd->type == CMD_HEREDOC) && \
			!minishell->input_fd && dup2(((t_cmd_redir *)cmd)->fd, 0))
			minishell->input_fd = true;
		if (((t_cmd_redir *)cmd)->fd > -1)
			close(((t_cmd_redir *)cmd)->fd);
		cmd = ((t_cmd_redir *)cmd)->cmd;
	}
	if (!minishell->output_fd && read == 1 && minishell->pipe_fds[1] > -1)
		dup2(minishell->pipe_fds[1], STDOUT_FILENO);
	if (!minishell->input_fd && !read && minishell->pipe_read_store > -1)
		dup2(minishell->pipe_read_store, STDIN_FILENO);
	if (minishell->pipe_fds[0] > -1)
		close(minishell->pipe_fds[0]);
	if (minishell->pipe_fds[1] > -1)
		close(minishell->pipe_fds[1]);
	if (minishell->pipe_read_store > -1)
		close(minishell->pipe_read_store);
	minishell->cmd = cmd;
}

char	*find_cmd(char *cmd, t_env *env)
{
	char	*final_cmd;
	char	**paths;
	int		index;

	final_cmd = NULL;
	while (env && ft_strncmp(env->key, "PATH", 5))
		env = env->next;
	if (!env)
		return (ft_putendl_fd("PATH is not found.", 2), cmd);
	paths = ft_split(env->value, ':');
	if (!paths)
		return (perror("Finding the command"), cmd);
	index = -1;
	while (paths[++index])
	{
		final_cmd = ft_char_strjoin(paths[index], cmd, '/');
		if (!final_cmd)
			return (ft_free_2d_arr(paths), perror("Finding the command"), cmd);
		if (!access(final_cmd, X_OK))
			break ;
		free(final_cmd);
		final_cmd = NULL;
	}
	return (ft_free_2d_arr(paths), final_cmd);
}

char	**convert_command(t_cmd *cmd, t_token *current)
{
	char	**str_tokens;
	int		token_count[2];

	token_count[0] = 0;
	token_count[1] = 0;
	while (cmd->type > CMD_PIPE && cmd->type < CMD_AND)
		cmd = ((t_cmd_redir *)cmd)->cmd;
	current = ((t_cmd_exec *)cmd)->tokens;
	if (!current)
		return (NULL);
	while (current && ++token_count[0])
		current = current->next;
	str_tokens = ft_calloc(++token_count[0], sizeof(char *));
	if (!str_tokens)
		return (perror("Command conversion"), NULL);
	current = ((t_cmd_exec *)cmd)->tokens;
	while (current)
	{
		str_tokens[token_count[1]] = ft_strdup(current->content);
		if (!str_tokens[token_count[1]++])
			return (free_split(str_tokens, token_count[0]), \
				perror("Command conversion"), NULL);
		current = current->next;
	}
	return (str_tokens);
}

bool	confirm_command(char **cmd, t_env *env)
{
	char	*cmd_original;

	if (!ft_strchr(*cmd, '/'))
	{
		cmd_original = *cmd;
		*cmd = find_cmd(*cmd, env);
		if (!*cmd)
			return (ft_putstr_fd(cmd_original, 2), *cmd = cmd_original, \
				g_code = 127, ft_putendl_fd(": command not found", 2), false);
		free(cmd_original);
	}
	else
	{
		if (access(*cmd, F_OK) == -1)
			return (ft_putstr_fd(*cmd, 2), g_code = 127,
				ft_putendl_fd(": no such file or directory", 2), false);
		else if (access(*cmd, X_OK) == -1)
			return (ft_putstr_fd(*cmd, 2), g_code = 126,
				ft_putendl_fd(": permission denied", 2), false);
	}
	return (true);
}

/**
 * @brief Checks if the command is a builtin.
 *
 * It goes through a bunch of if and else statements that call ft_strncmp()
 * with different builtin names. If one of them return a difference of 0, the
 * function returns a positive.
 *
 * @param str is the name of the command that will be compared.
 *
 * @return true if the command name is a builtin and false it it not.
 *
 */
bool	is_builtin(t_bltn *builtin, char *str)
{
	if (!ft_strncmp(str, "cd", 3))
		return (*builtin = CD, true);
	else if (!ft_strncmp(str, "echo", 5))
		return (*builtin = ECHO, true);
	else if (!ft_strncmp(str, "env", 4))
		return (*builtin = ENV, true);
	else if (!ft_strncmp(str, "exit", 5))
		return (*builtin = EXIT, true);
	else if (!ft_strncmp(str, "export", 7))
		return (*builtin = EXPORT, true);
	else if (!ft_strncmp(str, "pwd", 4))
		return (*builtin = PWD, true);
	else if (!ft_strncmp(str, "unset", 6))
		return (*builtin = UNSET, true);
	return (*builtin = NONE, false);
}

/**
 * @brief Executes the correct builtin.
 *
 * It goes through a bunch of if and else statements that call ft_strncmp()
 * with different builtin names. If one of them return a difference of 0, the
 * corresponding function is called.
 *
 * @param cmd contains the name of the command that will be compared.
 * @param minishell is sent to the builtins as required.
*/
bool	exec_builtin(char **cmd, t_minishell *minishell)
{
	if (minishell->bltin == CD)
		return (ft_cd(cmd, minishell), true);
	else if (minishell->bltin == ECHO)
		return (ft_echo(cmd), true);
	else if (minishell->bltin == ENV)
		return (ft_env(cmd, minishell->envp), true);
	// else if (minishell->builin_command == EXIT)
	// 	return (ft_exit(cmd, minishell), true);
	else if (minishell->bltin == EXPORT)
		return (ft_export(minishell, cmd), true);
	else if (minishell->bltin == PWD)
		return (ft_pwd(cmd), true);
	else if (minishell->bltin == UNSET)
		return (ft_unset(minishell, cmd), true);
	return (false);
}

/**
 * @brief Creates a child process and executes the specified command
 * Accepts an array of strings as input that will be passed to execve
 * following the format:
 * {command, options}
 */
void	exec_cmd(t_minishell *minishell, char **cmd, int read)
{
	int		pid;
	int		exit_code;

	receive_signal(CHILD);
	pid = fork();
	if (pid == 0)
	{
		duplicate_fds(minishell->cmd, minishell, read);
		if (!exec_builtin(cmd, minishell) && execve(cmd[0], cmd, minishell->envp))
			perror("execve() failed");
		free_char_command(cmd);
		free_parsing(minishell);
		free_environment(minishell);
		exit(WEXITSTATUS(errno));
	}
	waitpid(pid, &exit_code, 0);
	g_code = WEXITSTATUS(exit_code);
}

void	run_command(t_minishell *minishell, int read)
{
	t_cmd	*cmd1;
	char	**cmd;

	cmd1 = minishell->cmd;
	if (!cmd1)
		return ;
	if (cmd1->type != CMD_PIPE && cmd1->type != CMD_AND && cmd1->type != CMD_OR)
	{
		cmd = convert_command(cmd1, NULL);
		if (!cmd)
			return ;
		is_builtin(&minishell->bltin, *cmd);
		if (minishell->bltin == UNSET || minishell->bltin == EXIT || \
			minishell->bltin == CD || (minishell->bltin == EXPORT && cmd[1]))
			exec_builtin(cmd, minishell);
		else if (minishell->bltin != NONE || \
			confirm_command(&(*cmd), minishell->env_variables))
			exec_cmd(minishell, cmd, read);
		free_char_command(cmd);
	}
	// else if (cmd1->type == CMD_PIPE || cmd1->type == CMD_AND || 
	// 	cmd1->type == CMD_OR)
	// 	exec_pipe(minishell, env);
}
