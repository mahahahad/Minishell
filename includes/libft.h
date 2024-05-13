/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdanish <mdanish@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 15:29:04 by mdanish           #+#    #+#             */
/*   Updated: 2024/05/13 18:59:10 by mdanish          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdbool.h>

char	*ft_char_strjoin(char const *s1, char const *s2, char const c);
void	ft_free_2d_arr(char **arr);
char	ft_is_quotation(char c, char quote);
bool	ft_isspace(char c);
char	*ft_join(char **tab);
void	*ft_memset(void *array, int character, size_t bytes);
int		ft_printarr(char **tab);
char	**ft_split(char const *s, char c);
char	*ft_strchr(const char *s, int c);
void	ft_strcpy(char *dest, char const *src);
char	*ft_strdup(const char *src);
char	*ft_strjoin(char const *s1, char const *s2);
size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize);
size_t	ft_strlen(const char *str);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
char	*ft_substr(char const *s, unsigned int start, size_t len);

#endif
