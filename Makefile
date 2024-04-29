NAME = minishell
C_FLAGS = -Wall -Werror -Wextra -Iincludes
ADDITIONAL_FLAGS = -lreadline -I/usr/include -g3

SRCS = main.c
OBJS_DIR = objs/
OBJS = $(addprefix $(OBJS_DIR), $(SRCS:.c=.o))
UTILS = utils/*.c

all: $(NAME)

$(NAME): $(UTILS) $(OBJS)
	@cc $(C_FLAGS) $(OBJS) $(UTILS) $(ADDITIONAL_FLAGS) -o $(NAME)
	@echo "Compiled $(NAME)"

$(OBJS_DIR)%.o: %.c
	@mkdir -p $(OBJS_DIR)
	@cc $(C_FLAGS) -c $< -o $@

clean:
	@rm -rf **/*.o
	@rm -rf $(OBJS_DIR)
	@echo "Removed object files"

fclean: clean
	@rm -rf $(NAME)
	@echo "Removed executable"

re: fclean all

.PHONY: all clean fclean re
