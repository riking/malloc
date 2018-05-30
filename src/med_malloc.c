/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   med_malloc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyork <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/22 18:18:15 by kyork             #+#    #+#             */
/*   Updated: 2018/05/30 13:19:00 by kyork            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc_private.h"

#define OCCUPIED 0xAAAAAAAAAAAAAAABULL

/*
** OCCUPIED has a bit pattern of 10 10 10 ... 10 11
**
** max size is 32 slots - anything larger is a HUGE alloc
** slot count needs to be rounded up to next pow2
**
** each slot takes 2 bits in the bitmap
** 3 = start of allocation; 2 = continuation of allocation, 0 = free
*/

static int				get_slot_count(size_t size)
{
	size = CEILDIV(size, 256);
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

	maskval = atomic_load_explicit(set, memory_order_acquire);
	if ((maskval & OCCUPIED) == OCCUPIED)
		return (-1);
	chkmask = (size == 32) ? OCCUPIED : OCCUPIED & ((1ULL << (size * 2)) - 1);
	max = (max >= 32) ? 64 : max * 2;
	bitidx = 0;
	while (bitidx + (size * 2) <= max)
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
	size_t			idx;
	ssize_t			r;

	idx = 0;
	while (idx < page->item_count)
	{
		r = bitmask_claim(pg_bitset_ptr(page, idx), size,
				(((ssize_t)page->item_count) - (idx & ~31)));
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

	sz = get_slot_count(bytes);
	while (1)
	{
		pthread_rwlock_rdlock(&g->zoneinfo_lock);
		idx = -1;
		mem = NULL;
		while (!mem && ++idx < g->zoneinfo_count)
		{
			if (g->zoneinfo[idx].item_class == SZ_MEDIUM_256)
				mem = reservem(&g->zoneinfo[idx], sz);
		}
		if (mem)
			atomic_fetch_add(&g->zoneinfo[idx].alloc_count, 1);
		pthread_rwlock_unlock(&g->zoneinfo_lock);
		if (mem)
			return (mem);
		if (0 != more_pages(g, SZ_MEDIUM_256))
			return (NULL);
	}
}
