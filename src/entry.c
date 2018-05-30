/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   entry.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyork <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/23 12:40:51 by kyork             #+#    #+#             */
/*   Updated: 2018/05/29 18:53:55 by kyork            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc_private.h"
#include "malloc_api.h"

#include <errno.h>

t_mglobal			g_mglobal = {
	PTHREAD_RWLOCK_INITIALIZER,
	NULL, 0, 0,
	PTHREAD_MUTEX_INITIALIZER,
	PTHREAD_ONCE_INIT,
	false,
	false,
	0,
};

PRIVATE_VOID		malloc_setup_stub(void)
{
	malloc_setup(&g_mglobal);
}

EXPORT_VOIDSTAR		malloc(size_t size)
{
	void			*newptr;

	if (!ACCESS_ONCE(g_mglobal.init_done))
		pthread_once(&g_mglobal.init_once, malloc_setup_stub);
	newptr = do_malloc(&g_mglobal, size);
	log_call(&g_mglobal, LOGT_MALLOC, newptr, size);
	if (!newptr)
		errno = ENOMEM;
	return (newptr);
}

EXPORT_VOID			free(void *ptr)
{
	ssize_t		size;

	if (!ACCESS_ONCE(g_mglobal.init_done))
		pthread_once(&g_mglobal.init_once, malloc_setup_stub);
	if (!ptr)
		return ;
	size = do_free(&g_mglobal, ptr);
	if (size < 0)
		log_call(&g_mglobal, LOGT_BADFREE, ptr, (size_t)size);
	else
		log_call(&g_mglobal, LOGT_FREE, ptr, (size_t)size);
}

EXPORT_VOIDSTAR		realloc(void *ptr, size_t size)
{
	void	*newptr;

	if (!ACCESS_ONCE(g_mglobal.init_done))
		pthread_once(&g_mglobal.init_once, malloc_setup_stub);
	(void)ptr;
	newptr = do_realloc(&g_mglobal, ptr, size);
	if (!newptr)
		errno = ENOMEM;
	return (newptr);
}

EXPORT_VOID			show_alloc_mem(void)
{
	if (!ACCESS_ONCE(g_mglobal.init_done))
		pthread_once(&g_mglobal.init_once, malloc_setup_stub);
	do_show_alloc_mem(&g_mglobal);
}
