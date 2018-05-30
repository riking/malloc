/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyork <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/23 16:23:59 by kyork             #+#    #+#             */
/*   Updated: 2018/05/29 18:00:11 by kyork            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc_private.h"

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
	if (!r)
		malloc_panicf("free() of an unmanaged pointer: %p", ptr);
	if (r->item_class == SZ_HUGE)
	{
		pthread_rwlock_unlock(&g->zoneinfo_lock);
		pthread_rwlock_wrlock(&g->zoneinfo_lock);
		size = huge_free(r, find_region(g, ptr));
	}
	else if (r->item_class == SZ_TINY_8 || r->item_class == SZ_TINY_64)
		size = small_free(r, pg_alloc_idx(r, ptr));
	else if (r->item_class == SZ_MEDIUM_256)
		size = med_free(r, pg_alloc_idx(r, ptr));
	else
		malloc_panic("find_region returned decommitted region");
	pthread_rwlock_unlock(&g->zoneinfo_lock);
	if (size < 0)
		log_call(g, LOGT_BADFREE, ptr, size);
	return (size);
}
