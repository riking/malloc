/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   med_free.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyork <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/23 16:55:39 by kyork             #+#    #+#             */
/*   Updated: 2018/05/30 13:28:54 by kyork            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc_private.h"

#define OCCUPIED 0xAAAAAAAAAAAAAAABULL
#define TEST     0xFFFFFFFFFFFFFFFFULL

static t_u64		pickbits(t_u64 mask, int bits)
{
	if (bits == 64)
		return (mask);
	return (mask & ((1ULL << bits) - 1));
}

/*
** size is in bitmask bits, not in slots (== slots * 2)
** first op with (max, offset) is convert slots -> bits
**
** test each allocation size against the bitmask
** when loop exits, size will be actual * 2
** if offset not actually allocated, size will be 2 -> div2 -> 1
** convert size from bits->slots = div2, so if offset not allocated ret 0
**
** @param offset slot offset
** @param max number of valid slots in the bitmask
** @returns number of occupied slots, or 0 if no allocation
*/

static int			med_detect_size(t_u64 maskval, t_u8 offset, int max)
{
	int		size_bits;
	int		off_bits;

	size_bits = 2;
	max = (max >= 32) ? 64 : max * 2;
	off_bits = offset * 2;
	while ((size_bits + off_bits) <= max)
	{
		if (((maskval >> off_bits) & pickbits(TEST, size_bits)) !=
					pickbits(OCCUPIED, size_bits))
			break ;
		size_bits = size_bits * 2;
	}
	size_bits = size_bits / 2;
	return (size_bits / 2);
}

ssize_t				med_getsize(t_region *page, void *ptr)
{
	t_u64	maskval;
	size_t	idx;
	int		size_slots;

	idx = pg_alloc_idx(page, ptr);
	maskval = atomic_load(pg_bitset_ptr(page, idx));
	size_slots = med_detect_size(maskval, idx % 32,
			(page->item_count - (idx & ~31)));
	if (size_slots == 0)
		return (-1);
	return (size_slots * SZ_MEDIUM_256);
}

ssize_t				med_free(t_region *page, size_t idx_slots)
{
	t_u64	maskval;
	t_u64	clearmask;
	t_u64	newmask;
	int		size_slots;

	maskval = atomic_load(pg_bitset_ptr(page, idx_slots));
	size_slots = med_detect_size(maskval, idx_slots % 32,
			(page->item_count - (idx_slots & ~31)));
	if (size_slots == 0)
		return (-1);
	clearmask = pickbits(TEST, size_slots * 2) << ((idx_slots % 32) * 2);
	while (1)
	{
		if ((maskval & clearmask) !=
				(pickbits(OCCUPIED, size_slots * 2) << ((idx_slots % 32) * 2)))
			malloc_panic("medium region has corrupted bitmask");
		newmask = maskval & ~clearmask;
		if (atomic_compare_exchange_strong(pg_bitset_ptr(page, idx_slots),
					&maskval, newmask))
		{
			atomic_fetch_add(&page->alloc_count, -1);
			return (size_slots * SZ_MEDIUM_256);
		}
	}
}

size_t				med_show(t_mglobal *g, t_region *page, int flags)
{
	t_u8	idx;
	int		size;
	size_t	total;

	idx = 0;
	total = 0;
	show_alloc(g, SHOW_ZONEHDR | flags, page->page, NULL);
	while (idx < page->item_count)
	{
		size = med_detect_size(atomic_load(pg_bitset_ptr(page, idx)), idx % 32,
				(page->item_count - (idx & ~31)));
		if (size == 0)
		{
			show_alloc(g, SHOW_ISFREE | flags,
					pg_alloc_ptr(page, idx), pg_alloc_ptr(page, idx + 1));
			idx++;
			continue ;
		}
		show_alloc(g, SHOW_ALLOCD | flags,
				pg_alloc_ptr(page, idx), pg_alloc_ptr(page, idx + size));
		total += SZ_MEDIUM_256 * size;
		idx += size;
	}
	return (total);
}
