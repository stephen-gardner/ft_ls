#!/usr/bin/make -f
NAME = ft_ls
CC = gcc
CFLAGS = -Wall -Werror -Wextra
INC = -I libft/inc
LIBFT = libft/libft.a
SRC = \
	ft_ls\
	load\
	parser\
	print\
	sort\
	stat\
	util\
	wrappers
OBJ = $(patsubst %, %.o, $(SRC))

all: $(NAME)

$(NAME): $(LIBFT) $(OBJ)
	@echo "Building $@..."
	@$(CC) $(CFLAGS) $(LIBFT) $(OBJ) -o $@
	@echo "Done."

$(LIBFT):
	@echo "Building $@..."
	@make -C libft

%.o: %.c
	@echo "Compiling $<..."
	@$(CC) $(CFLAGS) $(INC) -c $< -o $@

clean:
	@rm -rf $(OBJ)
	@make -C libft clean
	@echo "Object files removed."

fclean: clean
	@rm -f $(NAME)
	@make -C libft fclean
	@echo "$(NAME) removed."

re: fclean all
