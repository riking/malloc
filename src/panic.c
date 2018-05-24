/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   panic.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyork <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/23 15:02:20 by kyork             #+#    #+#             */
/*   Updated: 2018/05/23 17:32:45 by kyork            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdarg.h>

#include "malloc_private.h"
#include <ft_printf.h>
#include <libft.h>

#include <stdlib.h>
#include <unistd.h>

#define PANIC_PREFIX "*** ft_malloc[%d]: "
#define PANIC_LINE_1 PANIC_PREFIX "panicing!\n"
#define PANIC_LINE_FINAL PANIC_PREFIX "The program will now exit."
#define PREFIX "+++ ft_malloc[%d]: "

VOID_NORETURN		malloc_panicf(const char *fmt, ...)
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

VOID_NORETURN		malloc_panic(const char *str)
{
	malloc_panicf("%s", str);
}

void				log_call(t_mglobal *g, int which, void *ptr, size_t size)
{
	char	buf[1024];
	int		sz;

	(void)g;
	//if (!g->enable_logging)
	//	return ;
	sz = 0;
	if (which == LOGT_MALLOC)
		sz = ft_snprintf(buf, 1024, PREFIX "allocated %zd bytes at %p\n",
				getpid(), size, ptr);
	else if (which == LOGT_REALLOC)
		sz = ft_snprintf(buf, 1024, PREFIX "realloced %zd bytes to %p\n",
				getpid(), size, ptr);
	else if (which == LOGT_FREE)
		sz = ft_snprintf(buf, 1024, PREFIX "freed %zd bytes at %p\n",
				getpid(), size, ptr);
	write(2, buf, sz);
}
