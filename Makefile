# **************************************************************************** #
#                                                                              #
#                                                        ::::::::              #
#    Makefile                                          :+:    :+:              #
#                                                     +:+                      #
#    By: avaliull <avaliull@student.codam.nl>        +#+                       #
#                                                   +#+                        #
#    Created: 2025/02/21 20:02:30 by avaliull     #+#    #+#                   #
#    Updated: 2025/02/27 22:50:29 by avaliull     ########   odam.nl           #
#                                                                              #
# **************************************************************************** #

MAKEFLAGS	= -s

CFILES	=	pipex.c\
			pipex_utils.c\
			kindergarten.c\

OBJDIR = obj
SRCDIR = src
SRCDIRS = $(SRCDIR)

OFILES	= $(addprefix $(OBJDIR)/,$(CFILES:.c=.o))

VPATH	= $(INCLUDE) $(SRCDIRS)

LIBFT_PRINTF	= $(SRCDIR)/libft_printf/libftprintf.a
LIBFT_PRINTF_DIR = $(SRCDIR)/libft_printf

INCLUDEFILES = pipex.h
INCLUDE = inc $(LIBFT_PRINTF_DIR)

NAME	= pipex

RM	= rm -f

CC	= cc
CFLAGS	= -Wall -Wextra -Werror
INPUT	= infile "ls" "cat" outfile

clangd:
	$(MAKE) fclean
	intercept-build-14 make all

$(OBJDIR):
	mkdir $@
$(OBJDIR)/%.o: %.c $(INCLUDEFILES) | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@ $(addprefix -I,$(INCLUDE))

$(LIBFT_PRINTF):
	export CFLAGS
	$(MAKE) all -C $(LIBFT_PRINTF_DIR)

$(NAME): $(OFILES) $(LIBFT_PRINTF)
	$(CC) $(CFLAGS) -o $@ $^ $(addprefix -I,$(INCLUDE))

all: $(NAME)

libs_clean:
	$(MAKE) clean -C $(LIBFT_PRINTF_DIR)
clean: libs_clean
	$(RM) $(OFILES)
fclean:	clean
	$(RM) $(NAME)

re:	fclean all

debug: CFLAGS += -g
debug: re

gdb: debug
	gdb ./$(NAME)

test:	$(NAME)
	./$< $(INPUT)

leak:	debug
	valgrind -s --leak-check=full --show-leak-kinds=all \
	--track-fds=yes ./$(NAME) $(INPUT)

.PHONY:	clangd all clean fclean re libs_clean test leak debug gdb
