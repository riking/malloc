# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kyork <marvin@42.fr>                       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2016/10/09 19:11:26 by kyork             #+#    #+#              #
#    Updated: 2018/05/23 14:59:11 by kyork            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

NAME		= libft_malloc_$(HOSTTYPE).so

FILENAMES	+= entry.c setup.c
FILENAMES	+= pages.c pg_reserve.c
FILENAMES	+=
FILENAMES	+=

OBJS		= $(addprefix build/ft_malloc_, $(FILENAMES:.c=.o))
OBJS		+= libft/libft.a

CFLAGS		+= -Wall -Wextra -Wmissing-prototypes
#CFLAGS		= -Wall -Wextra -Wfloat-equal -Wundef -Wint-to-pointer-cast -Wshadow -Wpointer-arith -Wcast-align -Wstrict-prototypes -Wcast-qual -Wmissing-prototypes -Wstrict-overflow=5 -Wwrite-strings -Wconversion --pedantic-errors
CFLAGS		+= -I include/
CFLAGS		+= -fPIC -fvisibility=hidden
LDFLAGS		+= -fPIC -fvisibility=hidden
SYMBOLS		= malloc free realloc show_alloc_mem

ifndef NO_WERROR
	CFLAGS += -Werror
	LDFLAGS += -Werror
endif

.PHONY: all clean fclean re

all: $(NAME)

libft_malloc_x86_64_Linux.so: $(OBJS)
	$(CC) -o $@ -shared -fPIC -fvisibility=hidden $(OBJS)

libft_malloc_x86_64_Darwin.so: $(OBJS)
	$(CC) -o $@ -shared $(addprefix --for-linker=-exported_symbol --for-linker=_,$(SYMBOLS)) $(OBJS)

libft/libft.a:
	make -C libft libft.a 

clean:
	rm -rf build
	make -C libft clean
	@printf "\e[33m\e[1m[CLEAN]\e[m $$(basename $$(pwd))\n"

fclean: clean
	rm -f $(NAME)
	make -C libft fclean
	@printf "\e[33m\e[1m[FCLEAN]\e[m $$(basename $$(pwd))\n"

re: fclean
	$(MAKE) all

build/ft_malloc_%.o: src/%.c src/*.h
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c -o $@ $<

