# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kyork <marvin@42.fr>                       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2016/10/09 19:11:26 by kyork             #+#    #+#              #
#    Updated: 2018/05/09 17:30:05 by kyork            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

NAME		= libft_malloc_$(HOSTTYPE).so

FILENAMES	+=
FILENAMES	+=
FILENAMES	+=
FILENAMES	+=

OBJS		= $(addprefix build/ft_malloc_, $(FILENAMES:.c=.o))
OBJS		+= libft/libft.a

CFLAGS		+= -Wall -Wextra -Wmissing-prototypes
#CFLAGS		= -Wall -Wextra -Wfloat-equal -Wundef -Wint-to-pointer-cast -Wshadow -Wpointer-arith -Wcast-align -Wstrict-prototypes -Wcast-qual -Wmissing-prototypes -Wstrict-overflow=5 -Wwrite-strings -Wconversion --pedantic-errors
CFLAGS		+= -I includes/
LDFLAGS		+= -Wall -Wextra
CFLAGS		+= -fPIC -fvisibility=hidden
LDFLAGS		+= -shared -fPIC -fvisibility=hidden

ifndef NO_WERROR
	CFLAGS += -Werror
	LDFLAGS += -Werror
endif

.PHONY: all clean fclean re

all: $(NAME)

$(NAME): $(OBJS)
	$(LD) -o $@ $(LDFLAGS) $(OBJS)

libft/libft.a:
	make -C libft libft.a CFLAGS="-fvisibility=hidden"

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

