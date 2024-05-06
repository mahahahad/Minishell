#ifndef LIBFT_H
# define LIBFT_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdbool.h>

int		ft_printarr(char **tab);
char	**ft_split(char const *s, char c);
size_t	ft_strlen(const char *str);
char	*ft_substr(char const *s, unsigned int start, size_t len);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
char	*ft_strjoin(char const *s1, char const *s2);
void	ft_strcpy(char *dest, char const *src);
char	*ft_join(char **tab);
__attribute__((pure)) char	*ft_strchr(const char *s, int c);
void	ft_free_2d_arr(char **arr);
char	*ft_strdup(const char *src);
size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize);
char	*ft_char_strjoin(char const *s1, char const *s2, char const c);
char	ft_is_quotation(char c, char quote);
bool	ft_isspace(char c);

#endif
