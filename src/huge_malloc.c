/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   huge_malloc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyork <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/23 16:58:45 by kyork             #+#    #+#             */
/*   Updated: 2018/05/30 15:18:41 by kyork            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc_private.h"
#include <sys/mman.h>

static void		*grab_page(t_mglobal *g, t_region *page, size_t osize)
{
	size_t		roundsize;

	if ((osize & (g->pagesize - 1)) != 0)
		roundsize = (osize & ~(g->pagesize - 1)) + g->pagesize;
	else
		roundsize = osize;
	page->page = mmap(NULL, roundsize, XPROT_RW, XMAP_AP, -1, 0);
	if (!page->page || page->page == MAP_FAILED)
	{
		page->page = NULL;
		return (NULL);
	}
	page->size = roundsize;
	page->item_class = SZ_HUGE;
	page->item_count = osize;
	page->alloc_count = 1;
	page->bitset_bytes = 0;
	return (page->page);
}

void			*huge_malloc(t_mglobal *g, size_t size)
{
	void		*ptr;
	t_region	*page;
	size_t		idx;

	page = NULL;
	while (!page)
	{
		pthread_rwlock_wrlock(&g->zoneinfo_lock);
		idx = 0;
		while (!page && idx < g->zoneinfo_count)
		{
			if (g->zoneinfo[idx].item_class == SZ_DECOMMIT)
				page = &g->zoneinfo[idx];
			idx++;
		}
		if (page)
			ptr = grab_page(g, page, size);
		pthread_rwlock_unlock(&g->zoneinfo_lock);
		if (!page)
			if (0 != more_zoneinfo(g))
				return (NULL);
	}
	return (ptr);
}

/*
** note: huge_free holds writelock
*/

ssize_t			huge_free(t_region *page, void *ptr)
{
	size_t		size;

	if (page->page != ptr)
		return (-1);
	size = page->size;
	atomic_store(&page->alloc_count, 0);
	return (size);
}

size_t			huge_show(t_mglobal *g, t_region *page, int flags)
{
	show_alloc(g, SHOW_ZONEHDR | flags, page->page, NULL);
	show_alloc(g, SHOW_ALLOCD | flags, page->page,
			((char*)page->page) + page->item_count);
	return (page->size);
}
