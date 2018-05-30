/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   huge_malloc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyork <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/23 16:58:45 by kyork             #+#    #+#             */
/*   Updated: 2018/05/29 17:49:23 by kyork            ###   ########.fr       */
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
	if (!page->page)
		return (NULL);
	page->size = roundsize;
	page->item_class = SZ_HUGE;
	page->item_count = osize;
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
		}
		if (page)
			ptr = grab_page(g, page, size);
		pthread_rwlock_unlock(&g->zoneinfo_lock);
		if (!page)
			if (!more_zoneinfo(g))
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
	munmap(page->page, page->size);
	page->page = NULL;
	size = page->size;
	page->size = 0;
	page->item_class = SZ_DECOMMIT;
	return (size);
}

size_t			huge_show(t_region *page, int flags)
{
	show_alloc(SHOW_ZONEHDR | SHOW_LRGZONE | flags, page->page, NULL);
	show_alloc(SHOW_ALLOCD | flags, page->page,
			((char*)page->page) + page->size);
	return (page->size);
}
