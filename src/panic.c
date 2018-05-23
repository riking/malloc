/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   panic.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyork <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/23 15:02:20 by kyork             #+#    #+#             */
/*   Updated: 2018/05/23 15:12:06 by kyork            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdarg.h>

#include "malloc_private.h"
#include <ft_printf.h>
#include <libft.h>

#include <stdlib.h>
#include <unistd.h>

#define PANIC_LINE_1 "*** ft_malloc[%d]: panicing\n"
#define PANIC_PREFIX "*** ft_malloc[%d]: "
#define PANIC_LINE_FINAL "*** ft_malloc[%d]: The program will now exit."

void				malloc_panicf(const char *fmt, ...)
{
	char	buf[1024];
	int		pid;
	int		sz;
	va_list	args;

	pid = getpid();
	sz = ft_snprintf(buf, 1024, PANIC_LINE_1, pid);
	write(2, buf, sz);
	sz = ft_snprintf(buf, 1024, PANIC_PREFIX, pid);
	write(2, buf, sz);
	va_start(args, fmt);
	sz = ft_vsnprintf(buf, 1024, fmt, args);
	va_end(args);
	if (sz > 1023)
	{
		write(2, buf, sz);
		write(2, "...", 3);
	}
	else
		write(2, buf, sz);
	write(2, "\n", 1);
	sz = ft_snprintf(buf, 1024, PANIC_LINE_FINAL, pid);
	write(2, buf, sz);
	exit(127);
}

void				malloc_panic(const char *str)
{
	malloc_panicf("%s", str);
}

void				log_call(t_mglobal *g, int which, void *ptr, size_t size)
{
	(void)g;
	(void)which;
	(void)ptr;
	(void)size;
}
