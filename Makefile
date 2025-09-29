# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: glugo-mu <glugo-mu@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/09/11 18:10:13 by glugo-mu          #+#    #+#              #
#    Updated: 2025/09/11 19:39:58 by glugo-mu         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror -g
SRC = main.c builtins.c
OBJ = $(SRC:.C=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) -lreadline -o $(NAME) $(OBJ)


clean:
	rm -f $(OBJ)

re: fclean all

.PHONY: all clean fclean re