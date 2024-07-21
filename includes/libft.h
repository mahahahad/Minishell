/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdanish <mdanish@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 15:29:04 by mdanish           #+#    #+#             */
/*   Updated: 2024/07/21 16:40:27 by mdanish          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <stdint.h>
# include <unistd.h>
# include <stddef.h>

# define HEX_LOW "0123456789abcdef"
# define HEX_UP "0123456789ABCDEF"
# define DECIMAL "0123456789"

void	free_split(char **split, size_t word_count);
void	*ft_calloc(size_t count, size_t size);
char	*ft_c_strjoin_free(char *s1, char *s2, char c, int free_string);
char	*ft_char_strjoin(char const *s1, char const *s2, char const c);
void	ft_free_2d_arr(char **arr);
char	ft_is_quotation(char c);
int		ft_isalnum(int character);
int		ft_isalpha(int letter);
int		ft_isdigit(int character);
bool	ft_isspace(char c);
char	*ft_join(char **tab);
void	*ft_memcpy(void *destination, void *source, size_t bytes);
void	*ft_memset(void *array, int character, size_t bytes);
int		ft_printarr(char **tab);
void	ft_putchar_fd(char character, int fd);
void	ft_putendl_fd(char *string, int fd);
void	ft_putnbr_base_fd(long int number, int fd, char *base, int base_len);
void	ft_putnbr_fd(long int number, int fd);
void	ft_putstr_fd(char *string, int fd);
char	**ft_split(char const *s, char c);
char	*ft_strchr(const char *s, int c);
void	ft_strcpy(char *dest, char const *src);
char	*ft_strdup(const char *src);
char	*ft_strjoin_free(char *s1, char *s2, int free_string);
char	*ft_strjoin(char const *s1, char const *s2);
size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize);
size_t	ft_strlen(const char *str);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
char	*ft_substr(char const *s, unsigned int start, size_t len);
size_t	word_counter(char const *string, char delimiter);
size_t	word_length(char const *string, char delimiter);

#endif
