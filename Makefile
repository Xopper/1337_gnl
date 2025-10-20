# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ahaloua <marvin@42.fr>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/05/29 23:30:40 by ahaloua           #+#    #+#              #
#    Updated: 2019/06/26 19:36:06 by ahaloua          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = gnl_test
CC = gcc
CFLAGS = -Wall -Wextra -Werror -g
LIBFT = libft/libft.a

SRC = get_next_line.c main.c
OBJ = $(SRC:.c=.o)

all: $(NAME)

$(LIBFT):
	@make -C libft

$(NAME): $(LIBFT) $(OBJ)
	@$(CC) $(CFLAGS) $(OBJ) $(LIBFT) -o $(NAME)
	@echo "Compiled $(NAME)"

%.o: %.c get_next_line.h
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -f $(OBJ)
	@make -C libft clean
	@echo "Cleaned object files"

fclean: clean
	@rm -f $(NAME)
	@make -C libft fclean
	@echo "Cleaned executable"

re: fclean all

.PHONY: all clean fclean re
