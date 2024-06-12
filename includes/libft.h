/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdanish <mdanish@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 15:29:04 by mdanish           #+#    #+#             */
/*   Updated: 2024/06/04 11:56:25 by maabdull         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdbool.h>

# define HEX_LOW "0123456789abcdef"
# define HEX_UP "0123456789ABCDEF"
# define DECIMAL "0123456789"

void	ft_putchar_fd(char character, int fd);
void	ft_putendl_fd(char *string, int fd);
void	ft_putunbr_base_fd(long int number, int fd, char *base, int base_len);
void	ft_putnbr_fd(long int number, int fd);
void	ft_putstr_fd(char *string, int fd);
char	*ft_char_strjoin(char const *s1, char const *s2, char const c);
void	ft_free_2d_arr(char **arr);
char	ft_is_quotation(char c);
bool	ft_isspace(char c);
int	ft_isdigit(char c);
int	ft_isalnum(const char c);
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
