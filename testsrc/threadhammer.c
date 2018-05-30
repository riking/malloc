/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threadhammer.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyork <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/29 15:47:54 by kyork             #+#    #+#             */
/*   Updated: 2018/05/30 12:33:46 by kyork            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// gcc -o testthread testthread.c -L. -lft_malloc -L$(LIBFT_LOCATION) -lft

#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#include <libft.h>
#include <ft_printf.h>

const static int	g_sizes[] = {0, 3, 8, 16, 21, 1024, 4096, 70000};

void			show_alloc_mem(void);
volatile int	g_stop;

static void		*worker(void* arg)
{
	void		*ptrs[100];
	unsigned	seed;
	int			i;
	size_t		sz;

	i = 0;
	seed = (unsigned)(uintptr_t)arg;
	while (i < 100)
		ptrs[i++] = NULL;
	while (!g_stop)
	{
		i = (i + rand_r(&seed)) % 100;
		free(ptrs[i]);
		ptrs[i] = NULL;
		sz = rand_r(&seed) & 7;
		ptrs[i] = malloc(g_sizes[sz]);
		ft_memset(ptrs[i], 0x41, g_sizes[sz]);
	}
	i = 0;
	while (i < 100)
		free(ptrs[i++]);
	return (NULL);
}

int				main(void)
{
	pthread_t	threads[4];

	pthread_create(&threads[0], NULL, &worker, (void*)(uintptr_t)1);
	pthread_create(&threads[1], NULL, &worker, (void*)(uintptr_t)2);
	pthread_create(&threads[2], NULL, &worker, (void*)(uintptr_t)3);
	pthread_create(&threads[3], NULL, &worker, (void*)(uintptr_t)4);
	sleep(3);
	g_stop = 1;
	show_alloc_mem();
	pthread_join(threads[0], NULL);
	pthread_join(threads[1], NULL);
	pthread_join(threads[2], NULL);
	pthread_join(threads[3], NULL);
	ft_printf("AFTER FREE EVERYTHING:\n");
	show_alloc_mem();
}
