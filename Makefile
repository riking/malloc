# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kyork <marvin@42.fr>                       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2016/10/09 19:11:26 by kyork             #+#    #+#              #
#    Updated: 2018/05/30 10:17:18 by kyork            ###   ########.fr        #
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
CFLAGS		+= -I include/
CFLAGS		+= -fPIC -fvisibility=hidden -g

SYMBOLS		= malloc free realloc show_alloc_mem
COMMA		= ,
LDFLAGS		+= $(addprefix -Wl$(COMMA)-exported_symbol$(COMMA)_,$(SYMBOLS))
LDFLAGS		+= -lpthread

ifndef NO_WERROR
	CFLAGS += -Werror
endif

.PHONY: all clean fclean re

all: $(NAME)

libft_malloc.so: libft_malloc_$(HOSTTYPE).so
	ln -sf $< $@

libft_malloc_$(HOSTTYPE).so: $(OBJS) libft/libft.a
	$(CC) -o $@ -shared $(CFLAGS) $(OBJS) $(LDFLAGS)

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

testa1: testsrc/simple.c $(NAME)
	$(CC) $(CFLAGS) -o $@ $^ -L./libft -lft

testa2: testsrc/circlemalloc.c $(NAME)
	$(CC) $(CFLAGS) -o $@ $^ -L./libft -lft

testa3: testsrc/double_free.c $(NAME)
	$(CC) $(CFLAGS) -o $@ $^ -L./libft -lft

testa4: testsrc/alloc_way_too_much.c $(NAME)
	$(CC) $(CFLAGS) -o $@ $^ -L./libft -lft

testa5: testsrc/realloc_inplace.c $(NAME)
	$(CC) $(CFLAGS) -o $@ $^ -L./libft -lft

testc0: testsrc/no_allocs.c $(NAME)
	$(CC) $(CFLAGS) -o $@ $^ -L./libft -lft

testc1: testsrc/simple_1024.c $(NAME)
	$(CC) $(CFLAGS) -o $@ $^ -L./libft -lft

testc2: testsrc/simple_with_free.c $(NAME)
	$(CC) $(CFLAGS) -o $@ $^ -L./libft -lft
