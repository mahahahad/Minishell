PARSER_SRCS := $(addprefix $(PARSER_DIR)/, create_cmd.c create_token.c \
	dollar.c parser.c parser_utils.c parser_utils_2.c wildcard.c)
SRCS += $(PARSER_SRCS)
