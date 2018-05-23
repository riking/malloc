/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc_private.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyork <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/24 11:58:13 by kyork             #+#    #+#             */
/*   Updated: 2018/04/24 13:21:47 by kyork            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FTMALLOC_MALLOC_PRIVATE_H
# define FTMALLOC_MALLOC_PRIVATE_H

# include <pthread.h>

# ifndef __STDC_NO_ATOMICS__
#  include <stdatomic.h>
# else
#  error Atomic operations are required
# endif

# define ACCESS_ONCE(x) (*(volatile typeof(x) *)&(x))

# define MEDIUM_ALLOC_CUTOFF 256
# define HUGE_ALLOC_CUTOFF 4096

/*
** zoneinfo - array<t_region> protected by zoneinfo_lock
** standalone - array<t_standalone> for "huge" allocations
*/
typedef struct		s_mglobal {
	pthread_rwlock_t	zoneinfo_lock;
	t_array				zoneinfo;
	pthread_mutex_t		standalone_lock;
	t_array				standalone;

	size_t				pagesize;
}					t_mglobal;

extern t_mglobal	_malloc_g;

/*
** size - the size of the mmap()ed range, in bytes
** page - pointer to the beginning of the page(s)
** item_class_size - the allocation unit for this region
** The minimum value for bitset_bytes is 16 (128 bits), as zones must allow
** for at least 100 allocations.
*/
typedef struct		s_region {
	size_t				size;
	void				*page;
	size_t				item_class_size;
	int					bitset_bytes;
	_Atomic int			will_delete;
}					t_region;

typedef struct		s_standalone {

}					t_standalone;

size_t				get_size_class(size_t request);

#endif
