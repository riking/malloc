/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyork <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/23 13:01:58 by kyork             #+#    #+#             */
/*   Updated: 2018/05/23 13:58:31 by kyork            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc_private.h"
#include <errno.h>
#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>

#define CEILDIV(x, y) (((x) + (y) - 1) / (y))

void			malloc_setup(t_mglobal *g)
{
	if (!pthread_rwlock_init(&g->zoneinfo_lock, NULL))
		malloc_panic("could not initialize locks");
	g->pagesize = getpagesize();
	g->x_zoneinfo_bytes = g->pagesize * 2;
	g->zoneinfo = mmap(NULL, g->x_zoneinfo_bytes, PROT_READ | PROT_WRITE,
			MAP_ANON | MAP_PRIVATE, -1, 0);
	if (!g->zoneinfo)
		malloc_panicf("could not allocate initial memory: %s",
				sys_errlist[errno]);
	g->zoneinfo_count = (g->pagesize * 2) / sizeof(t_region);
	g->init_done = true;
}

int				more_zoneinfo(t_mglobal *g)
{
	void		*new_zoneinfo;
	size_t		new_bytes;
	size_t		new_count;

	pthread_rwlock_wrlock(&g->zoneinfo_lock);
	new_bytes = g->x_zoneinfo_bytes * 2;
	new_zoneinfo = mmap(NULL, new_bytes, PROT_READ | PROT_WRITE, MAP_ANON |
			MAP_PRIVATE, -1, 0);
	if (!new_zoneinfo)
	{
		pthread_rwlock_unlock(&g->zoneinfo_lock);
		return (1);
	}
	ft_memmove(new_zoneinfo, g->zoneinfo, g->x_zoneinfo_bytes);
	munmap(g->zoneinfo, g->x_zoneinfo_bytes);
	g->x_zoneinfo_bytes = new_bytes;
	g->zoneinfo_count = new_bytes / sizeof(t_region);
	g->zoneinfo = new_zoneinfo;
	pthread_rwlock_unlock(&g->zoneinfo_lock);
	return (0);
}
