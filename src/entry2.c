/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   entry2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyork <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/30 11:38:05 by kyork             #+#    #+#             */
/*   Updated: 2018/05/30 11:44:16 by kyork            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc_private.h"
#include "malloc_api.h"
#include <errno.h>

EXPORT_VOIDSTAR		calloc(size_t a, size_t b)
{
	size_t		size;
	void		*newptr;

	if (__builtin_mul_overflow(a, b, &size))
	{
		errno = ENOMEM;
		return (NULL);
	}
	if (!ACCESS_ONCE(g_mglobal.init_done))
		pthread_once(&g_mglobal.init_once, malloc_setup_stub);
	newptr = do_calloc(&g_mglobal, size);
	log_call(&g_mglobal, LOGT_MALLOC, newptr, size);
	if (!newptr)
		errno = ENOMEM;
	return (newptr);
}

EXPORT_VOIDSTAR		valloc(size_t size)
{
	void		*newptr;

	if (!ACCESS_ONCE(g_mglobal.init_done))
		pthread_once(&g_mglobal.init_once, malloc_setup_stub);
	newptr = huge_malloc(&g_mglobal, size);
	log_call(&g_mglobal, LOGT_MALLOC, newptr, size);
	if (!newptr)
		errno = ENOMEM;
	return (newptr);
}

EXPORT_VOIDSTAR		reallocf(void *ptr, size_t size)
{
	void	*newptr;

	if (!ACCESS_ONCE(g_mglobal.init_done))
		pthread_once(&g_mglobal.init_once, malloc_setup_stub);
	newptr = do_realloc(&g_mglobal, ptr, size);
	if (!newptr)
		errno = ENOMEM;
	return (newptr);
}
