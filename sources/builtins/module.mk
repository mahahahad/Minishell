BUILTIN_SRCS := $(addprefix $(BUILTIN_DIR)/, ft_cd.c ft_export.c \
				ft_export_utils.c ft_unset.c simple_builtins.c)
SRCS += $(BUILTIN_SRCS)
