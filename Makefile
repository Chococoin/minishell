# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: glugo-mu <glugo-mu@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/09/11 18:10:13 by glugo-mu          #+#    #+#              #
#    Updated: 2025/10/08 10:46:32 by glugo-mu         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror -g
INCLUDES = -I includes
SRC = \
	src/main.c \
	src/env/copy_env.c \
	src/utils/utils.c \
	src/parsing/split_input.c \
	src/parsing/command_builder.c \
	src/parsing/command_utils.c \
	src/parsing/token_list.c \
	src/parsing/token_utils.c \
	src/parsing/tokenizer.c
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
