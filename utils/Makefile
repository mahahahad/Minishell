NAME = libutils.a
C_FLAGS = -Wall -Werror -Wextra -I../includes

BGREEN = \033[1;32m
BYELLOW = \033[1;33m
RESET = \033[0m\033[K

SRCS = ft_split.c ft_strlen.c ft_substr.c

OBJS_DIR = objs/
OBJS = $(addprefix $(OBJS_DIR), $(SRCS:.c=.o))

all : $(NAME)

$(NAME) : $(OBJS)
	@ar -rc $(NAME) $(OBJS)
	@echo "$(BGREEN)Created Utils Library$(RESET)"

$(OBJS_DIR)%.o : %.c
	@mkdir -p $(OBJS_DIR)
	@cc $(C_FLAGS) -c $< -o $@

clean :
	@rm -rf objs *.o

fclean : clean
	@rm -f $(NAME)

re : fclean all

.PHONY : all clean fclean re
