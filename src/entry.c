/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   entry.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyork <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/23 12:40:51 by kyork             #+#    #+#             */
/*   Updated: 2018/05/23 13:01:42 by kyork            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc_private.h"
#include "malloc_api.h"

#include <errno.h>

t_mglobal			g_mglobal = {
	{0},
	NULL, 0,
	PTHREAD_ONCE_INIT,
	false,
	0,
};

void				malloc_setup_stub(void)
{
	malloc_setup(&g_mglobal);
}

EXPORT(void *)		malloc(size_t size)
{
	void	*newptr;

	if (!ACCESS_ONCE(g_mglobal.init_done))
		pthread_once(&g_mglobal.init_once, malloc_setup_stub);
	newptr = do_malloc(&g_mglobal, size);
	log_call(&g_mglobal, LOGT_MALLOC, newptr, size);
	if (!newptr)
		errno = ENOMEM;
	return (newptr);
}

EXPORT(void)		free(void *ptr)
{
	int		logcode;

	if (!ACCESS_ONCE(g_mglobal.init_done))
		pthread_once(&g_mglobal.init_once, malloc_setup_stub);
	if (!ptr)
		return ;
	do_free(&g_mglobal, ptr);
}

EXPORT(void *)		realloc(void *ptr, size_t size)
{
	void	*newptr;

	if (!ACCESS_ONCE(g_mglobal.init_done))
		pthread_once(&g_mglobal.init_once, malloc_setup_stub);
	newptr = do_realloc(&g_mglobal, ptr, size);
	log_call(&g_mglobal, LOGT_REALLOC, newptr, size);
	if (!newptr)
		errno = ENOMEM;
	return (newptr);
}

EXPORT(void)		show_alloc_mem(void)
{
	if (!ACCESS_ONCE(g_mglobal.init_done))
		pthread_once(&g_mglobal.init_once, malloc_setup_stub);
	do_show_alloc_mem(&g_mglobal);
}
