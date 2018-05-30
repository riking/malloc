/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   show_mem.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyork <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/23 17:05:10 by kyork             #+#    #+#             */
/*   Updated: 2018/05/29 19:38:56 by kyork            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc_private.h"
#include "malloc.h"
#include <unistd.h>
#include <ft_printf.h>

#define PLUS_STR32 "++++++++++++++++++++++++++++++++"

void			show_alloc(int flags, void *start, void *end)
{
	char		buf[50];
	int			sz;

	if ((flags & SHOW_ALLOCD))
	{
		sz = ft_snprintf(buf, 50, "%p - %p : ", start, end);
		write(1, buf, sz);
		sz = ft_snprintf(buf, 50, "%zd bytes\n", ((char*)end) - ((char*)start));
		write(1, buf, sz);
	}
	else if ((flags & SHOW_ZONEHDR))
	{
		if ((flags & SHOW_SMLZONE))
			sz = ft_snprintf(buf, 50, "TINY : %p\n", start);
		else if ((flags & SHOW_MEDZONE))
			sz = ft_snprintf(buf, 50, "SMALL : %p\n", start);
		else if ((flags & SHOW_LRGZONE))
			sz = ft_snprintf(buf, 50, "LARGE : %p\n", start);
		else
			malloc_panic("show-alloc bad flags");
		write(1, buf, sz);
	}
}

static int		sortfn(void *left, void *right, size_t size, void *data)
{
	t_region	*a;
	t_region	*b;

	(void)data;
	(void)size;
	a = *(t_region**)left;
	b = *(t_region**)right;
	if (a->page < b->page)
		return (-1);
	else if (a->page > b->page)
		return (1);
	return (0);
}

static void		find_of_class(t_mglobal *g, t_array *sortspace,
					t_size_class cls)
{
	size_t		zidx;
	size_t		aidx;

	zidx = 0;
	aidx = 0;
	while (zidx < g->zoneinfo_count)
	{
		if (g->zoneinfo[zidx].item_class == cls || ((cls == SZ_TINY_8) &&
					g->zoneinfo[zidx].item_class == SZ_TINY_64))
			((void**)sortspace->ptr)[aidx++] = &g->zoneinfo[zidx];
		zidx++;
	}
	sortspace->item_count = aidx;
	ft_ary_sort(sortspace, &sortfn, NULL);
}

static size_t	print_of_class(t_mglobal *g, t_array *ary,
		t_size_class cls, int flags)
{
	size_t		aidx;
	size_t		total;
	void		*ptr;

	(void)g;
	aidx = 0;
	total = 0;
	if (ary->item_count > 0)
	{
		ptr = (((t_region**)ary->ptr)[aidx])->page;
		show_alloc(SHOW_ZONEHDR | flags, ptr, NULL);
	}
	while (aidx < ary->item_count)
	{
		if (cls == SZ_TINY_8)
			total += small_show(((t_region**)ary->ptr)[aidx], 0);
		if (cls == SZ_MEDIUM_256)
			total += med_show(((t_region**)ary->ptr)[aidx], 0);
		if (cls == SZ_HUGE)
			total += huge_show(((t_region**)ary->ptr)[aidx], 0);
		aidx++;
	}
	return (total);
}

size_t			do_show_alloc_mem(t_mglobal *g)
{
	void		*scratch[g->zoneinfo_count];
	t_array		sorting;
	size_t		total;

	sorting = (t_array){scratch, sizeof(void*), 0, g->zoneinfo_count};
	total = 0;
	find_of_class(g, &sorting, SZ_TINY_8);
	total += print_of_class(g, &sorting, SZ_TINY_8, SHOW_SMLZONE);
	find_of_class(g, &sorting, SZ_MEDIUM_256);
	total += print_of_class(g, &sorting, SZ_MEDIUM_256, SHOW_MEDZONE);
	find_of_class(g, &sorting, SZ_HUGE);
	total += print_of_class(g, &sorting, SZ_HUGE, SHOW_LRGZONE);
	return (total);
}
