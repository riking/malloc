/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bitset_offset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyork <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/23 15:29:47 by kyork             #+#    #+#             */
/*   Updated: 2018/05/30 15:05:35 by kyork            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc_private.h"
#include "malloc_api.h"

/*
** note: alloc_ptr allows for idx == item_count
** which creates a pointer to the end of the page
*/

void				*pg_alloc_ptr(t_region *page, size_t idx)
{
	volatile char	*ptr;

	if (page->item_class < SZ_TINY_8 || page->item_class > SZ_MEDIUM_256)
		malloc_panic("pg_alloc_ptr called on non-allocmask page");
	ptr = ((char*)page->page) + page->bitset_bytes +
		(((size_t)page->item_class) * idx);
	if (ptr > (((char*)page->page) + page->size))
		malloc_panic("pg_alloc_ptr calculated a location outside page");
	return ((void*)ptr);
}

size_t				pg_alloc_idx(t_region *page, const void *ptr)
{
	ptrdiff_t		offset;

	if (page->item_class < SZ_TINY_8 || page->item_class > SZ_MEDIUM_256)
		malloc_panic("pg_alloc_idx called on non-allocmask page");
	offset = (((char*)ptr) - (((char*)page->page) + page->bitset_bytes));
	return (offset / page->item_class);
}

ATOM_U64			*pg_bitset_ptr(t_region *page, size_t idx)
{
	volatile char	*ptr;

	if (page->item_class == SZ_TINY_8 || page->item_class == SZ_TINY_64)
		ptr = ((char*)page->page) + (8 * (idx / 64));
	else if (page->item_class == SZ_MEDIUM_256)
		ptr = ((char*)page->page) + (8 * (idx / 32));
	else
		malloc_panic("pg_bitset_ptr called on non-allocmask page");
	return ((ATOM_U64*)ptr);
}

t_size_class		get_size_class(size_t size)
{
	if (size <= SZ_TINY_8)
		return (SZ_TINY_8);
	if (size <= SZ_TINY_64)
		return (SZ_TINY_64);
	if (size < SZ_HUGE)
		return (SZ_MEDIUM_256);
	return (SZ_HUGE);
}

EXPORT_SIZET		malloc_good_size(size_t size)
{
	t_size_class	cls;

	cls = get_size_class(size);
	if (cls == SZ_HUGE)
		return (size);
	if (cls == SZ_MEDIUM_256)
		return (med_slot_count(size) * SZ_MEDIUM_256);
	return (cls);
}
