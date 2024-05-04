NAME := minishell
CC := cc
CFLAGS := -Wall -Werror -Wextra -Iincludes
ADDITIONAL_FLAGS := -lreadline -I/usr/include -g3

# Directories
SRCS_DIR := sources
OBJS_DIR := objects
SUB_DIRS := $(shell find $(SRCS_DIR) -type d -print)
UTILS_DIR := $(SRCS_DIR)/utils
PARSER_DIR := $(SRCS_DIR)/parsing
EXEC_DIR := $(SRCS_DIR)/execution
BUILTIN_DIR := $(SRCS_DIR)/builtins
SETUP_DIR := $(SRCS_DIR)/setup
CLEANUP_DIR := $(SRCS_DIR)/cleanup
MODULES := $(UTILS_DIR) $(PARSER_DIR) $(EXEC_DIR) $(BUILTIN_DIR) $(SETUP_DIR) $(CLEANUP_DIR)

# Files
SRCS := $(addprefix $(SRCS_DIR)/, main.c)
# Include the files needed by all the modules and append them to the SRCS variable
include $(patsubst %, %/module.mk, $(MODULES))
OBJS := $(SRCS:$(SRCS_DIR)%.c=$(OBJS_DIR)%.o)

# Custom colours
YELLOW := \033[38;2;230;255;0;48;2;25;25;25;1m
GREEN := \033[38;2;170;255;0;48;2;25;25;25;1m
RESET := \033[0m

# Rules
all: $(NAME)

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) $(ADDITIONAL_FLAGS) -o $(NAME)
	@echo "$(RESET)\n$(GREEN)Compiled $(NAME)$(RESET)"

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.c | $(OBJS_DIR)
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "$(YELLOW).\c$(RESET)"

$(OBJS_DIR):
	@mkdir -p $(patsubst $(SRCS_DIR)%, $(OBJS_DIR)%, $(SUB_DIRS))

clean:
	@rm -rf $(OBJS_DIR) $(OBJS)
	@echo "$(YELLOW)Removed object files$(RESET)"

fclean: clean
	@rm -rf $(NAME)
	@echo "$(YELLOW)Removed executable$(RESET)"

re: fclean all

debug: re
	@$(CC) $(CFLAGS) $(SRCS) $(ADDITIONAL_FLAGS) -g3 -fsanitize=address -o $(NAME)

.PHONY: all clean fclean re debug
