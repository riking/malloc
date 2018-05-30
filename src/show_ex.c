/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   show_ex.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyork <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/30 13:25:21 by kyork             #+#    #+#             */
/*   Updated: 2018/05/30 14:11:55 by kyork            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc_private.h"
#include "malloc.h"
#include <unistd.h>
#include <ft_printf.h>

static void			linewrap_putchar(t_mglobal *g, char c)
{
	write(1, &c, 1);
	g->show_temp++;
	if (g->show_temp == 64)
	{
		write(1, "\n", 1);
		g->show_temp = 0;
	}
}

static void			show_ex_med(t_mglobal *g, int flags, ptrdiff_t size)
{
	int		i;

	if (flags & SHOW_ISFREE)
	{
		linewrap_putchar(g, '.');
		return ;
	}
	linewrap_putchar(g, '#');
	i = 1;
	while (i < size)
	{
		linewrap_putchar(g, '+');
		i++;
	}
}

#define F_CMS_A (SHOW_CMPLX | SHOW_SMLZONE | SHOW_ALLOCD)
#define F_CMS_F (SHOW_CMPLX | SHOW_SMLZONE | SHOW_ISFREE)
#define F_CMM (SHOW_CMPLX | SHOW_MEDZONE)

void				show_ex(t_mglobal *g, int flags, void *start, void *end)
{
	char	buf[50];
	int		sz;

	if ((flags & F_CMS_A) == F_CMS_A)
		linewrap_putchar(g, '#');
	else if ((flags & F_CMS_F) == F_CMS_F)
		linewrap_putchar(g, '.');
	else if ((flags & F_CMM) == F_CMM)
		show_ex_med(g, flags, (((char*)end) - (char*)start) / 256);
	else if (flags & SHOW_ALLOCD)
	{
		sz = ft_snprintf(buf, 50, "%p - %p : ", start, end);
		write(1, buf, sz);
		sz = ft_snprintf(buf, 50, "%zd bytes\n", ((char*)end) - ((char*)start));
		write(1, buf, sz);
	}
	else if (flags & SHOW_TOTAL)
	{
		sz = ft_snprintf(buf, 50, "Total : %zd bytes\n", (uintptr_t)start);
		write(1, buf, sz);
	}
}
