NAME = minishell
C_FLAGS = -Wall -Werror -Wextra -I./includes
ADDITIONAL_FLAGS = -lreadline -I/usr/include -g3

B_GREEN = \033[1;32m
B_YELLOW = \033[1;33m
B_RED = \033[1;31m
BG_GREEN = \033[0;102m
RESET = \033[0m

SRCS = main.c
OBJS_DIR = objs/
OBJS = $(addprefix $(OBJS_DIR), $(SRCS:.c=.o))

all: $(NAME)

$(NAME): $(OBJS)
	@cc $(C_FLAGS) $(OBJS) $(ADDITIONAL_FLAGS) -o $(NAME)
	@printf "$(B_GREEN)Compiled $(NAME)$(RESET)"

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
