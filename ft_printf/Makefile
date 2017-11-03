#!/usr/bin/make -f
NAME = libftprintf.a
CC = gcc
CFLAGS = -Wall -Werror -Wextra
INC = -I inc
SRC_DIR = src
SRC = \
	char_print\
	char_util\
	float_print\
	float_util\
	ft_isdigit\
	ft_islower\
	ft_isupper\
	ft_memalloc\
	ft_memcpy\
	ft_memset\
	ft_printf\
	ft_printf_parser\
	ft_strdup\
	ft_strlen\
	ft_strnlen\
	ft_strupcase\
	ft_tolower\
	ft_toupper\
	int_handlers\
	int_handlers_deprecated\
	int_print\
	int_util
OBJ_DIR = $(SRC_DIR)/obj
OBJ = $(patsubst %, $(OBJ_DIR)/%.o, $(SRC))

all: $(NAME)

$(NAME): $(OBJ)
	@echo "Building $@..."
	@ar -rcs $@ $(OBJ)
	@echo "Done."

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	@echo "Compiling $<..."
	@$(CC) $(CFLAGS) $(INC) -c $< -o $@

clean:
	@rm -rf $(OBJ_DIR)
	@echo "Object files removed."

fclean: clean
	@rm -f $(NAME)
	@echo "$(NAME) removed."

re: fclean all
