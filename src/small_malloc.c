/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   small_malloc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyork <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/22 18:18:15 by kyork             #+#    #+#             */
/*   Updated: 2018/05/23 15:43:46 by kyork            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc_private.h"

/*
** TODO - randomize iteration order to reduce conflicts
*/

static ssize_t			bitmask_claim(ATOM_U64 *set)
{
	t_u64			maskval;
	t_u64			newval;
	int				bitidx;

	maskval = atomic_load_explicit(set, memory_order_acquire);
	while (1)
	{
		if (maskval == (-1ULL))
			return (-1);
		bitidx = __builtin_ctzl(~maskval);
		newval = maskval | (1 << bitidx);
		if (atomic_compare_exchange_weak(set, &maskval, newval))
			return (bitidx);
	}
}

static void				*reserve1(t_region *page)
{
	ssize_t			idx;
	ssize_t			r;

	idx = 0;
	while (idx < page->item_count)
	{
		r = bitmask_claim(pg_bitset_ptr(page, idx));
		if (r != -1)
			break ;
		idx += 64;
	}
	if (r != -1)
		return (pg_alloc_ptr(page, (idx + r)));
	return (NULL);
}

void					*small_malloc(t_mglobal *g, t_size_class cls)
{
	size_t		idx;
	void		*mem;

	while (1)
	{
		pthread_rwlock_rdlock(&g->zoneinfo_lock);
		idx = -1;
		mem = NULL;
		while (++idx < g->zoneinfo_count)
		{
			if (g->zoneinfo[idx].item_class == cls)
				mem = reserve1(&g->zoneinfo[idx]);
		}
		pthread_rwlock_unlock(&g->zoneinfo_lock);
		if (mem)
			return (mem);
		if (more_pages(g, cls) < 0)
			return (NULL);
	}
}
