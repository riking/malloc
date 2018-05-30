/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   show_mem.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyork <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/23 17:05:10 by kyork             #+#    #+#             */
/*   Updated: 2018/05/29 17:21:02 by kyork            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc_private.h"
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

size_t			do_show_alloc_mem(t_mglobal *g)
{
	size_t		idx;
	size_t		total;

	total = 0;
	pthread_rwlock_wrlock(&g->zoneinfo_lock);
	idx = 0;
	while (idx < g->zoneinfo_count)
	{
		if (g->zoneinfo[idx].item_class == SZ_HUGE)
			total += huge_show(&g->zoneinfo[idx], 0);
		else if (g->zoneinfo[idx].item_class == SZ_MEDIUM_256)
			total += med_show(&g->zoneinfo[idx], 0);
		else if (g->zoneinfo[idx].item_class == SZ_MEDIUM_256)
			total += small_show(&g->zoneinfo[idx], 0);
		idx++;
	}
	pthread_rwlock_unlock(&g->zoneinfo_lock);
	return (total);
}
