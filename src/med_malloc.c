/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   med_malloc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyork <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/22 18:18:15 by kyork             #+#    #+#             */
/*   Updated: 2018/05/23 17:38:19 by kyork            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc_private.h"

#define OCCUPIED 0xEAAAAAAAAAAAAAAAULL

/*
** max size is 32 slots - anything larger is a HUGE alloc
** slot count needs to be rounded up to next pow2
**
** each slot takes 2 bits in the bitmap
** 3 = start of allocation; 2 = continuation of allocation, 0 = free
*/

static int				round_pow2(t_u8 size)
{
	size--;
	size |= size >> 1;
	size |= size >> 2;
	size |= size >> 4;
	size++;
	return (size);
}

static ssize_t			bitmask_claim(ATOM_U64 *set, t_u8 size, int max)
{
	t_u64			maskval;
	t_u64			newval;
	t_u64			chkmask;
	t_u8			bitidx;

	chkmask = OCCUPIED >> (64 - (size * 2));
	maskval = atomic_load_explicit(set, memory_order_acquire);
	if ((maskval & OCCUPIED) == OCCUPIED)
		return (-1);
	max = (max >= 32) ? 64 : max * 2;
	bitidx = 0;
	while (bitidx + size <= max)
	{
		if ((maskval & (chkmask << bitidx)) == 0)
		{
			newval = maskval | (chkmask << bitidx);
			if (atomic_compare_exchange_strong(set, &maskval, newval))
				return (bitidx / 2);
		}
		bitidx += size;
	}
	return (-1);
}

static void				*reservem(t_region *page, int size)
{
	ssize_t			idx;
	ssize_t			r;

	idx = 0;
	while (idx < page->item_count)
	{
		r = bitmask_claim(pg_bitset_ptr(page, idx), size, (page->item_count - idx));
		if (r != -1)
			break ;
		idx += 32;
	}
	if (r != -1)
		return (pg_alloc_ptr(page, (idx + r)));
	return (NULL);
}

void					*med_malloc(t_mglobal *g, size_t bytes)
{
	size_t		idx;
	int			sz;
	void		*mem;

	sz = bytes / 256;
	if (sz == 0)
		sz = 1;
	sz = round_pow2(sz);
	while (1)
	{
		pthread_rwlock_rdlock(&g->zoneinfo_lock);
		idx = -1;
		mem = NULL;
		while (++idx < g->zoneinfo_count && !mem)
		{
			if (g->zoneinfo[idx].item_class == SZ_MEDIUM_256)
				mem = reservem(&g->zoneinfo[idx], sz);
		}
		pthread_rwlock_unlock(&g->zoneinfo_lock);
		if (mem)
			return (mem);
		if (0 != more_pages(g, SZ_MEDIUM_256))
			return (NULL);
	}
}
