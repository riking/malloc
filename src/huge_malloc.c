/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   huge_malloc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyork <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/23 16:58:45 by kyork             #+#    #+#             */
/*   Updated: 2018/05/23 17:04:37 by kyork            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc_private.h"
#include <sys/mman.h>

void			*huge_malloc(t_mglobal *g, size_t size)
{
	(void)g;
	(void)size;
	return (NULL);
}

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
	// TODO ^ this isn't locked...
	return (size);
}
