# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: glugo-mu <glugo-mu@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/09/11 18:10:13 by glugo-mu          #+#    #+#              #
#    Updated: 2025/10/31 16:20:47 by glugo-mu         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror -g
INCLUDES = -I includes
SRC =	src/main.c src/env/copy_env.c src/utils/utils.c \
		src/parsing/split_input.c src/parsing/command_builder.c \
		src/parsing/command_utils.c src/parsing/token_list.c \
		src/parsing/token_utils.c src/parsing/tokenizer.c src/parsing/expand.c \
		src/parsing/validate.c src/libft/ft_isalpha.c \
		src/libft/ft_isdigit.c src/libft/ft_isalnum.c src/libft/ft_isascii.c \
		src/libft/ft_isprint.c src/libft/ft_strlen.c src/libft/ft_memset.c \
		src/libft/ft_bzero.c src/libft/ft_memcpy.c src/libft/ft_memmove.c \
		src/libft/ft_strlcpy.c src/libft/ft_strlcat.c src/libft/ft_toupper.c \
		src/libft/ft_tolower.c src/libft/ft_strchr.c src/libft/ft_strrchr.c \
		src/libft/ft_strncmp.c src/libft/ft_memchr.c src/libft/ft_memcmp.c \
		src/libft/ft_strnstr.c src/libft/ft_atoi.c src/libft/ft_calloc.c \
		src/libft/ft_strdup.c src/libft/ft_putnbr_fd.c src/libft/ft_putstr_fd.c \
		src/libft/ft_putendl_fd.c src/libft/ft_putchar_fd.c src/libft/ft_substr.c \
		src/libft/ft_strjoin.c src/libft/ft_strjoin_char.c src/libft/ft_strtrim.c \
		src/libft/ft_split.c src/libft/ft_itoa.c src/libft/ft_striteri.c \
		src/libft/ft_strmapi.c 
OBJDIR = obj
OBJ = $(patsubst src/%.c,$(OBJDIR)/%.o,$(SRC))

all: $(OBJDIR) $(NAME)
	@echo "\033[1;33m"
	@echo "        ███╗   ███╗██╗███╗   ██╗██╗██╗  ██╗███████╗██╗     ██╗"
	@echo "        ████╗ ████║██║████╗  ██║██║██║  ██║██╔════╝██║     ██║\033[38;5;208m"
	@echo "        ██╔████╔██║██║██╔██╗ ██║██║███████║█████╗  ██║     ██║"
	@echo "        ██║╚██╔╝██║██║██║╚██╗██║██║██╔══██║██╔══╝  ██║     ██║\033[0;31m"
	@echo "        ██║ ╚═╝ ██║██║██║ ╚████║██║██║  ██║███████╗███████╗███████╗"
	@echo "        ╚═╝     ╚═╝╚═╝╚═╝  ╚═══╝╚═╝╚═╝  ╚═╝╚══════╝╚══════╝╚══════╝\033[0m"

$(NAME): $(OBJ)
	@$(CC) $(CFLAGS) -lreadline -o $(NAME) $(OBJ)

$(OBJDIR):
	@mkdir -p $(OBJDIR)

$(OBJDIR)/%.o: src/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)
	@if [ -d "$(OBJDIR)" ]; then find "$(OBJDIR)" -mindepth 1 -delete; fi

re: fclean all

# Debug / tooling targets
# - debug: recompile with -g -O0 for readable backtraces
# - gdb: launch gdb with the debug binary
# - asan: compile with AddressSanitizer and UBSan
# - valgrind: run valgrind on the debug binary
# - enable-coredump: instructions to enable core dumps in the current shell

debug: CFLAGS += -g -O0
debug: re

gdb: debug
	@echo "Starting gdb: run, then when it crashes use 'bt' to get backtrace"
	@gdb --args ./$(NAME)

asan: CFLAGS += -g -O0 -fsanitize=address,undefined -fno-omit-frame-pointer
asan: re
	@echo "Run ./$(NAME) to see ASAN/UBSAN output."

valgrind: debug
	@echo "Running valgrind --leak-check=full --track-origins=yes"
	@valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(NAME)

enable-coredump:
	@echo "To enable core dumps in this shell run: ulimit -c unlimited"
	@echo "Then run ./$(NAME) and, if a core is generated, use: gdb ./$(NAME) core"

.PHONY: all clean fclean re debug gdb asan valgrind enable-coredump
