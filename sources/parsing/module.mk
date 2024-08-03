PARSER_SRCS := $(addprefix $(PARSER_DIR)/, create_cmd.c create_token.c \
	dollar.c ft_print_error.c parser.c parser_utils.c wildcard.c)
SRCS += $(PARSER_SRCS)
