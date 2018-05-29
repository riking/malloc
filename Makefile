# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kyork <marvin@42.fr>                       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2016/10/09 19:11:26 by kyork             #+#    #+#              #
#    Updated: 2018/05/29 16:08:54 by kyork            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

NAME		= libft_malloc.so
LIBNAME		= ft_malloc_$(HOSTTYPE)

FILENAMES	+= entry.c setup.c panic.c free.c
FILENAMES	+= pages.c small_malloc.c bitset_offset.c
FILENAMES	+= med_malloc.c med_free.c
FILENAMES	+= huge_malloc.c show_mem.c

OBJS		= $(addprefix build/ft_malloc_, $(FILENAMES:.c=.o))
OBJS		+= libft/libft.a

CFLAGS		+= -Wall -Wextra -Wmissing-prototypes
#CFLAGS		= -Wall -Wextra -Wfloat-equal -Wundef -Wint-to-pointer-cast -Wshadow -Wpointer-arith -Wcast-align -Wstrict-prototypes -Wcast-qual -Wmissing-prototypes -Wstrict-overflow=5 -Wwrite-strings -Wconversion --pedantic-errors
CFLAGS		+= -I include/
CFLAGS		+= -fPIC -fvisibility=hidden -g
SYMBOLS		= malloc free realloc show_alloc_mem

ifndef NO_WERROR
	CFLAGS += -Werror
	LDFLAGS += -Werror
endif

.PHONY: all clean fclean re

all: $(NAME)

libft_malloc.so: libft_malloc_$(HOSTTYPE).so
	ln -s $< $@

libft_malloc_x86_64_Linux.so: $(OBJS)
	$(CC) -o $@ -shared -fPIC -fvisibility=hidden $(OBJS)

libft_malloc_x86_64_Darwin.so: $(OBJS)
	$(CC) -o $@ -shared $(CFLAGS) $(OBJS) $(addprefix --for-linker=-exported_symbol --for-linker=_,$(SYMBOLS)) -lpthread

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

test1: testsrc/simple.c $(NAME)
	$(CC) $(CFLAGS) -o $@ $^ -L. -l$(LIBNAME) -L./libft -lft

test2: testsrc/circlemalloc.c $(NAME)
	$(CC) $(CFLAGS) -o $@ $^ -L. -l$(LIBNAME) -L./libft -lft

test3: testsrc/double_free.c $(NAME)
	$(CC) $(CFLAGS) -o $@ $^ -L. -l$(LIBNAME) -L./libft -lft

test4: testsrc/alloc_way_too_much.c $(NAME)
	$(CC) $(CFLAGS) -o $@ $^ -L. -l$(LIBNAME) -L./libft -lft
