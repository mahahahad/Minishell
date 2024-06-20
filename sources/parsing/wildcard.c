/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdanish <mdanish@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 18:51:40 by mdanish           #+#    #+#             */
/*   Updated: 2024/06/21 01:55:39 by mdanish          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Searches for a wildcard.
 * 
 * The function goes through the token character by character to check if a
 * [ * ] is present. It ensures that the [ * ] enclosed within quotation is
 * ignored. Upon identification of the [ * ], the opendir() function is used to
 * open the current directory. It uses the return of getcwd() as its argument.
 * This only happens in the first call to this function.
 * 
 * @param token is the token that needs to be checked for the wildcard.
 * @param location will contain the location of the identified wildcard.
 * @param cwd points to the variable which will store the directory stream.
 * 
 * @return true when a [ * ] is found, false if no [ * ] or opendir() failed.
 */
static bool	wildcards_are_present(char *token, int *location, DIR **cwd)
{
	char	cwd_path[PATH_MAX];

	*location = -1;
	while (token[++*location])
	{
		if (ft_is_quotation(token[*location]))
			continue ;
		else if (token[*location] == '*')
			break ;
	}
	if (cwd && token[*location] == '*')
	{
		*cwd = opendir(getcwd(cwd_path, PATH_MAX));
		if (*cwd)
		{
			errno = 0;
			return (true);
		}
		ft_putendl_fd("Directory can not be opened", 2);
	}
	return (false);
}

/**
 * @brief Checks if expansion needs to include hidden files.
 * 
 * The function simply checks through a bunch of hard-coded cases to check if
 * any of the hidden files (files that begin with a [ . ]) are required to be
 * included in the expansion.
 * 
 * @param token will be checked to see if hidden files are needed.
 * @param location of the [ * ] in the token.
 * 
 * @return true if hidden files are to be included, false if otherwise.
 */
static bool	require_hidden_file(char *token, int location)
{
	if (location == 1 && token[0] == '.')
		return (true);
	if (location == 3 && token[1] == '.')
	{
		if (token[0] == '\'' && token[2] == '\'')
			return (true);
		if (token[0] == '"' && token[2] == '"')
			return (true);
	}
	if (location == 2 && token[0] == '.' && token[1] == '.')
		return (true);
	if (location == 4 && token[1] == '.' && token[2] == '.')
	{
		if (token[0] == '\'' && token[3] == '\'')
			return (true);
		if (token[0] == '"' && token[3] == '"')
			return (true);
	}
	return (false);
}

/**
 * @brief Compares the leading characters of the filename with the token.
 * 
 * The function goes through the token and matches them to the filename till
 * either the wildcard's location is reached or a non-matching character is
 * found. This also ensures it is done before the wildcard is reached.
 * 
 * It ensures that all quotes that are meant to be omitted are not included in
 * the matching of the token and filename.
 * 
 * @param token will be compared with the file.
 * @param file is the name of the file in the directory.
 * @param t_index points to the index of the token.
 * @param location is the index of the token at which the wildcard exists.
 * 
 * @return The index of the file at the end, or -1 in case of mismatch.
 */
static int	compare_names(char *token, char *file, int *t_index, int location)
{
	char	quote;
	int		f_index;

	quote = '\0';
	f_index = 0;
	while (token[++*t_index] && *t_index != location)
	{
		if (!quote && (token[*t_index] == '"' || token[*t_index] == '\''))
			quote = token[*t_index];
		else if (quote && token[*t_index] == quote)
			quote = '\0';
		else if (token[*t_index] != file[f_index++])
			return (-1);
	}
	return (f_index);
}

/**
 * @brief Checks if the file name matches with the token.
 * 
 * The function ensures that hidden files are needed by running the
 * require_hidden_file() function. It then sends it to compare_names() to check
 * if the file name matches the token. If it does match, it continues to set up
 * the next check by increasing the token's index and file's index as necessary.
 * 
 * To check is there are more [ * ] in the token, wildcards_are_present() is
 * called again. If the location points to a null byte, the evaluated return of
 * the ft_strncmp() function is returned from the function. If the location ends
 * up pointing to a [ * ], the a recursive call to match_pattern() is returned.
 * 
 * @param token will be used to see if the file is to be expanded to.
 * @param location contains the index of the [ * ] in the token.
 * @param file is the file name with will be matched with the token.
 * 
 * @return true if file matches the pattern, false if file doesn't.
 */
static bool	match_pattern(char *token, int location, char *file)
{
	int		f_index;
	int		t_index;

	if (file[0] == '.' && !require_hidden_file(token, location))
		return (false);
	t_index = -1;
	f_index = compare_names(token, file, &t_index, location);
	if (t_index != location || f_index < 0)
		return (false);
	while (token[t_index] == '*')
		t_index++;
	if (!token[t_index])
		return (true);
	while ((f_index || !location) && file[f_index] && \
		token[t_index] != file[f_index])
		f_index++;
	if (!file[f_index])
		return (false);
	wildcards_are_present(token + t_index, &location, NULL);
	if (!token[t_index + location])
		return (!ft_strncmp(token + t_index, file + f_index, location + 1));
	return (match_pattern(token + t_index, location, file + f_index));
}

/**
 * @brief Expands any and all wildcards if found.
 * 
 * The function uses wildcards_are_present() to identify if the token contains
 * any wildcards. This function will also store the index at which the wildcard
 * is at by updating the variable which will be sent to it. In the case that
 * there is no wildcard, the original token is returned.
 * 
 * The readdir() function returns information about the files present in the
 * current directory. This is sent to match_pattern() to validate the file name
 * according to the pattern provided by the token.
 * 
 * For each valid filename, it is joined and stored in the token variable, which
 * is returned upon success.
 * 
 * @param token is the token that needs to be checked for the wildcards.
 * @param store contains a copy of token.
 * 
 * @return the pointer to the final token.
 */
char	*wildcards(char *token, char *store)
{
	DIR		*cwd_stream;
	t_dir	*files;
	int		location;

	if (!wildcards_are_present(token, &location, &cwd_stream))
		return (token);
	token = NULL;
	files = readdir(cwd_stream);
	while (files)
	{
		if (match_pattern(store, location, files->d_name))
		{
			token = ft_c_strjoin_free(files->d_name, token, ' ', 2);
			if (!token)
				break ;
		}
		files = readdir(cwd_stream);
	}
	closedir(cwd_stream);
	if (errno)
		ft_putendl_fd("Directory can not be read", 2);
	else if (!errno && token)
		return (token);
	return (store);
}
