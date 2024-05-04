UTILS_SRCS := $(addprefix $(UTILS_DIR)/, ft_join.c ft_printarr.c \
	      ft_split.c ft_strchr.c ft_strcpy.c ft_strjoin.c ft_strlen.c \
	      ft_strncmp.c ft_substr.c ft_free_2d_arr.c ft_strlcpy.c ft_strdup.c ft_char_strjoin.c ft_is_quotation.c ft_isspace.c)
# UTILS_OBJS := $(UTILS_SRCS:$(UTILS_DIR)/%.c=$(OBJS_DIR)/%.o)
SRCS += $(UTILS_SRCS)
