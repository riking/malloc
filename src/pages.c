/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pages.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyork <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/23 14:10:12 by kyork             #+#    #+#             */
/*   Updated: 2018/05/23 15:40:46 by kyork            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc_private.h"
#include "pages.h"
#include <sys/mman.h>

/*
** bitset size cannot be smaller than alloc alignment
*/

const static t_pagesizes	g_pagesize_dfns[] = {
	{SZ_TINY_8, 8192, 1008, 128},
	{SZ_TINY_64, 8192, 127, 64},
	{SZ_MEDIUM_256, 32768, 127, 256},
};

#define RET_FOUND 1
#define RET_SKIP 0
#define RET_ERROR -1

static int					grab_page(t_region *r, const t_pagesizes *dfn)
{
	if (r->item_class == SZ_FREE && r->size == dfn->size)
	{
		r->item_class = dfn->item_class;
		r->item_count = dfn->item_count;
		r->bitset_bytes = dfn->bitset_bytes;
		return (RET_FOUND);
	}
	if (r->item_class != SZ_DECOMMIT)
		return (RET_SKIP);
	r->page = mmap(NULL, dfn->size, XPROT_RW, XMAP_AP, -1, 0);
	if (!r->page)
		return (RET_ERROR);
	r->size = dfn->size;
	r->item_class = dfn->item_class;
	r->item_count = dfn->item_count;
	r->bitset_bytes = dfn->bitset_bytes;
	return (RET_FOUND);
}

int							more_pages(t_mglobal *g, t_size_class cls)
{
	const t_pagesizes	*dfn;
	int					r;
	size_t				idx;

	dfn = (cls == SZ_TINY_8) ? &g_pagesize_dfns[0] : &g_pagesize_dfns[1];
	if (cls == SZ_MEDIUM_256)
		dfn = &g_pagesize_dfns[2];
	while (1)
	{
		pthread_rwlock_wrlock(&g->zoneinfo_lock);
		idx = 0;
		while (idx < g->zoneinfo_count)
		{
			r = grab_page(&g->zoneinfo[idx], dfn);
			if (r)
				break ;
			idx++;
		}
		pthread_rwlock_unlock(&g->zoneinfo_lock);
		if (r)
			return (r - 1);
		if (!more_zoneinfo(g))
			return (-1);
	}
}
