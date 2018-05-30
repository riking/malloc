/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyork <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/23 16:23:59 by kyork             #+#    #+#             */
/*   Updated: 2018/05/29 18:59:14 by kyork            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc_private.h"

/*
** do_malloc exists solely so that realloc can call it
*/

void			*do_malloc(t_mglobal *g, size_t size)
{
	t_size_class	cls;
	void			*newptr;

	cls = get_size_class(size);
	if (cls == SZ_TINY_8 || cls == SZ_TINY_64)
		newptr = small_malloc(g, cls);
	else if (cls == SZ_MEDIUM_256)
		newptr = med_malloc(g, size);
	else
		newptr = huge_malloc(g, size);
	return (newptr);
}

t_region		*find_region(t_mglobal *g, char *ptr)
{
	size_t		idx;
	t_region	*found;
	const char	*pg;

	idx = 0;
	found = NULL;
	while (!found && idx < g->zoneinfo_count)
	{
		if (g->zoneinfo[idx].item_class >= SZ_MIN_VALID)
		{
			pg = g->zoneinfo[idx].page;
			if (ptr > pg && ptr < (pg + g->zoneinfo[idx].size))
				found = &g->zoneinfo[idx];
		}
		idx++;
	}
	return (found);
}

ssize_t			do_free(t_mglobal *g, void *ptr)
{
	t_region	*r;
	ssize_t		size;

	pthread_rwlock_rdlock(&g->zoneinfo_lock);
	r = find_region(g, ptr);
	if (r)
	{
		if (r->item_class == SZ_HUGE)
		{
			pthread_rwlock_unlock(&g->zoneinfo_lock);
			pthread_rwlock_wrlock(&g->zoneinfo_lock);
			size = huge_free(find_region(g, ptr), ptr);
		}
		else if (r->item_class == SZ_TINY_8 || r->item_class == SZ_TINY_64)
			size = small_free(r, pg_alloc_idx(r, ptr));
		else if (r->item_class == SZ_MEDIUM_256)
			size = med_free(r, pg_alloc_idx(r, ptr));
		else
			malloc_panic("find_region returned invalid region");
	}
	pthread_rwlock_unlock(&g->zoneinfo_lock);
	if (!r)
		return (-1);
	return (size);
}

/*
** returns size of existing allocation; also responsible for bookkeeping
** of HUGE reallocs
*/

static ssize_t	realloc_getsize(t_mglobal *g, void *ptr, size_t newsize)
{
	t_region	*r;

	if (!ptr)
		return (0);
	r = find_region(g, ptr);
	if (!r)
		return (-1);
	if (r->item_class == SZ_TINY_8 || r->item_class == SZ_TINY_64)
		return (r->item_class);
	if (r->item_class == SZ_MEDIUM_256)
		return (med_getsize(r, ptr));
	if (r->item_class == SZ_HUGE)
	{
		if (newsize <= r->size)
			r->item_count = newsize;
		return (r->size);
	}
	malloc_panic("find_region returned invalid region");
}

/*
** step 1: check for in-place upgrade/downgrade / invalid ptr
** step 2: allocate new memory
*/

void			*do_realloc(t_mglobal *g, void *ptr, size_t newsize)
{
	ssize_t		exsize;
	void		*newptr;

	pthread_rwlock_wrlock(&g->zoneinfo_lock);
	exsize = realloc_getsize(g, ptr, newsize);
	pthread_rwlock_unlock(&g->zoneinfo_lock);
	if (exsize == -1 || ((size_t)exsize) <= newsize)
	{
		if (exsize == -1)
			log_call(g, LOGT_BADREALLOC, ptr, newsize);
		else
			log_call(g, LOGT_REALLOC_IP, ptr, newsize);
		return ((exsize == -1) ? NULL : ptr);
	}
	newptr = do_malloc(g, newsize);
	if (!newptr)
		return (NULL);
	ft_memcpy(newptr, ptr, exsize);
	do_free(g, ptr);
	pthread_mutex_lock(&g->print_lock);
	log_callb(g, LOGT_REALLOC_OLD, ptr, exsize);
	log_callb(g, LOGT_REALLOC_NEW, newptr, newsize);
	pthread_mutex_unlock(&g->print_lock);
	return (newptr);
}
