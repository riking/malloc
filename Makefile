# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kyork <marvin@42.fr>                       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2016/10/09 19:11:26 by kyork             #+#    #+#              #
#    Updated: 2018/04/24 11:53:59 by kyork            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= libft_malloc.a

FILENAMES	+= 
FILENAMES	+= 
FILENAMES	+= 
FILENAMES	+= 

OBJS		= $(addprefix build/ft_malloc_, $(FILENAMES:.c=.o))

CFLAGS		+= -Wall -Wextra -Wmissing-prototypes
CFLAGS		+= -I includes/
LDFLAGS		+= -Wall -Wextra

ifndef NO_WERROR
	CFLAGS += -Werror
	LDFLAGS += -Werror
endif

.PHONY: all clean fclean re

all: $(NAME)

$(NAME): $(OBJS)
	ar rcs $(NAME) $(OBJS)
	@printf "\e[32m\e[1m[OK]\e[m $$(basename $@)\n"

clean:
	rm -rf build
	@printf "\e[33m\e[1m[CLEAN]\e[m $$(basename $$(pwd))\n"

fclean: clean
	rm -f $(NAME)
	@printf "\e[33m\e[1m[FCLEAN]\e[m $$(basename $$(pwd))\n"

re: fclean
	$(MAKE) all

build:
	mkdir -p build

build/ft_malloc_%.o: srcs/%.c srcs/*.h
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c -o $@ $<

build/%.o: tests/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c -o $@ $<

smalltest: build/small_test.o $(NAME) | build
	$(CC) $(LDFLAGS) -o $@ build/small_test.o -L. -lft_malloc

nm_test: build/nm_test.o $(NAME)
	$(CC) $(LDFLAGS) -o $@ build/nm_test.o -L. -lft_malloc

fuzzmain: build/fuzz_main.o $(NAME) | build
	$(CC) $(LDFLAGS) -o $@ build/small_test.o -L. -lft_malloc
