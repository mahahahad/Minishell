/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdanish <mdanish@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 18:51:40 by mdanish           #+#    #+#             */
/*   Updated: 2024/08/02 17:28:35 by mdanish          ###   ########.fr       */
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
		cwd_path[0] = '\0';
		getcwd(cwd_path, PATH_MAX);
		*cwd = opendir(cwd_path);
		if (*cwd)
			return (errno = 0, true);
		perror("getcwd() in wildcard expansion");
		g_code = WEXITSTATUS(errno);
	}
	return (false);
}

/**
 * @brief Matches the trailing characters of the file to the token.
 * 
 * The function mainly uses the same logic as match_leaders() to compare the
 * bytes but in reverse. This means that it compares from the end of the string.
 * 
 * @param token will be used as the standard for comparison.
 * @param file is the filename that will be compared.
 * 
 * @return true if file matches the token, false if file does not.
 */
static bool	match_trailers(char *token, char *file)
{
	char	quote;
	int		bytes;
	int		index;

	quote = '\0';
	bytes = ft_strlen(token);
	index = ft_strlen(file);
	while (bytes-- && index)
	{
		if (!quote && (token[bytes] == '"' || token[bytes] == '\''))
			quote = token[bytes];
		else if (quote && token[bytes] == quote)
			quote = '\0';
		else if (token[bytes] != file[--index])
			return (false);
	}
	if (!index)
		return (false);
	return (true);
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
 * It also makes sure that the hidden files are not included in the expansion
 * unless it is specifically required by the token.
 * 
 * @param token will be compared with the file.
 * @param file is the name of the file in the directory.
 * @param t_index points to the index of the token.
 * @param location is the index of the token at which the wildcard exists.
 * 
 * @return The index of the file at the end, or -1 in case of mismatch.
 */
static int	match_leaders(char *token, char *file, int *t_index, int location)
{
	char	quote;
	int		index;

	index = -1;
	while (file[0] == '.' && token[++index] && ft_strchr("'\"", token[index]))
	{
		if (token[index] != token[index + 1] && token[index + 1] != '.')
			return (-1);
		if (token[index] == token [index + 1])
			index++;
	}
	if (file[0] == '.' && token[index] != '.')
		return (-1);
	quote = '\0';
	index = 0;
	while (token[++*t_index] && *t_index != location)
	{
		if (!quote && (token[*t_index] == '"' || token[*t_index] == '\''))
			quote = token[*t_index];
		else if (quote && token[*t_index] == quote)
			quote = '\0';
		else if (token[*t_index] != file[index++])
			return (-1);
	}
	return (index);
}

/**
 * @brief Checks if the file name matches with the token.
 * 
 * The function ensures that hidden files are needed by running the
 * It then sends it to compare_names() to check if the file name matches the
 * token. If it does match, it continues to set up the next check by increasing
 * the token's index and file's index as necessary.
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

	t_index = -1;
	f_index = match_leaders(token, file, &t_index, location);
	if (t_index != location || f_index < 0)
		return (false);
	while (token[t_index] && ft_strchr("*'\"", token[t_index]))
		t_index++;
	if (!token[t_index])
		return (true);
	wildcards_are_present(token + t_index, &location, NULL);
	if (!token[t_index + location])
		return (match_trailers(token + t_index, file + f_index));
	while (file[f_index] && token[t_index] != file[f_index])
		f_index++;
	if (!file[f_index])
		return (false);
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
 * For each valid filename, add_token_back() is called with the new_token()
 * function to add that specific file name as an independant token to the final
 * list present in the master struct.
 * 
 * @param token is the token that needs to be checked for the wildcards.
 * @param store contains a copy of token.
 * 
 * @return the pointer to the final token.
 */
t_token	*wildcard_expansion(char *token_string)
{
	DIR		*cwd_stream;
	int		location;
	t_dir	*files;
	t_token	*token;

	if (!wildcards_are_present(token_string, &location, &cwd_stream))
		return (NULL);
	files = readdir(cwd_stream);
	token = NULL;
	while (files)
	{
		if (match_pattern(token_string, location, files->d_name))
		{
			add_token_back(&token, new_token(ft_strdup(files->d_name), NULL, \
				false));
			if (!token)
				break ;
		}
		files = readdir(cwd_stream);
	}
	closedir(cwd_stream);
	g_code = WEXITSTATUS(errno);
	if (errno)
		return (perror("Wildcard expansion"), free_tokens(&token), NULL);
	return (token);
}
