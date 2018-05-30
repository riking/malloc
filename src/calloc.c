/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   calloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyork <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/30 11:34:00 by kyork             #+#    #+#             */
/*   Updated: 2018/05/30 11:35:29 by kyork            ###   ########.fr       */
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

void			*do_calloc(t_mglobal *g, size_t size)
{
	void			*newptr;

	newptr = do_malloc(g, size);
	if (!newptr)
		return (newptr);
	ft_memset(newptr, 0, size);
	return (newptr);
}
