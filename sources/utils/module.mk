UTILS_SRCS := $(addprefix $(UTILS_DIR)/, ft_calloc.c ft_char_strjoin.c \
			ft_free_2d_arr.c ft_is_quotation.c ft_isalnum.c ft_isalpha.c \
			ft_isdigit.c ft_isspace.c ft_join.c ft_lstadd_back.c ft_memcpy.c \
			ft_memset.c ft_printarr.c ft_putchar_fd.c ft_putendl_fd.c \
			ft_putnbr_base_fd.c ft_putnbr_fd.c ft_putstr_fd.c ft_split.c \
			ft_strchr.c ft_strcpy.c ft_strdup.c ft_strjoin.c ft_strjoin_free.c \
			ft_strlcpy.c ft_strlen.c ft_strncmp.c ft_substr.c)
SRCS += $(UTILS_SRCS)
