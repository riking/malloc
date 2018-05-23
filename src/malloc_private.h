/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc_private.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyork <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/24 11:58:13 by kyork             #+#    #+#             */
/*   Updated: 2018/05/23 14:56:30 by kyork            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FTMALLOC_MALLOC_PRIVATE_H
# define FTMALLOC_MALLOC_PRIVATE_H

# include <pthread.h>
# include <stddef.h>
# include <stdbool.h>
# include <sys/types.h>

# ifndef __STDC_NO_ATOMICS__
#  include <stdatomic.h>
# else
#  error Atomic operations are required
# endif

# include <libft.h>
# include <commontypes.h>

# define ACCESS_ONCE(x) (*(volatile typeof(x) *)&(x))
# define CEILDIV(x, y) (((x) + (y) - 1) / (y))

# define ATOM_U64 _Atomic t_u64

# define XPROT_RW (PROT_READ | PROT_WRITE)
# define XMAP_AP (MAP_ANON | MAP_PRIVATE)

/*
** SZ_DECOMMIT - page has been unmapped / never allocated
** SZ_FREE - page can be reused
*/

typedef enum		e_size_class {
	SZ_DECOMMIT = 0,
	SZ_FREE = 1,
	SZ_TINY_8 = 8,
	SZ_TINY_64 = 64,
	SZ_MEDIUM_256 = 256,
	SZ_HUGE = 8192,
	SZ_MIN_VALID = SZ_TINY_8,
}					t_size_class;

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
	t_s16				item_class;
	t_u16				item_count;
	t_s32				bitset_bytes;
}					t_region;

/*
** zoneinfo - array<t_region> protected by zoneinfo_lock
** pagesize - getpagesize() return
*/
typedef struct		s_mglobal {
	pthread_rwlock_t	zoneinfo_lock;
	t_region			*zoneinfo;
	size_t				zoneinfo_count;
	size_t				x_zoneinfo_bytes;

	pthread_once_t		init_once;
	volatile bool		init_done;

	t_u32				pagefree_cycle;
	size_t				pagesize;
}					t_mglobal;

void				malloc_setup(t_mglobal *g);
void				malloc_panic(const char *str);
void				malloc_panicf(const char *fmt, ...);

size_t				med_roundup(size_t request);

/*
** more_zoneinfo - increases the zoneinfo allocation
**   returns nonzero on error
** more_pages - allocates more pages for the given size class (no HUGE)
**   returns nonzero on error
** new_huge_page - make a new SZ_HUGE page of given size
*/

int					more_zoneinfo(t_mglobal *g);
int					more_pages(t_mglobal *g, t_size_class cls);
int					new_huge_page(t_mglobal *g, size_t size);

void				*pg_alloc_ptr(t_region *page, size_t idx);
ATOM_U64			*pg_bitset_ptr(t_region *page, size_t idx);

t_size_class		get_size_class(size_t request);
void				*small_malloc(t_mglobal *g, t_size_class cls);
void				*med_malloc(t_mglobal *g, size_t size);
void				*large_malloc(t_mglobal *g, size_t size);

t_region			*find_mem(void *ptr);

void				*do_malloc(t_mglobal *g, size_t size);
int					do_free(t_mglobal *g, void *ptr);
void				*do_realloc(t_mglobal *g, void *ptr, size_t newsize);
void				do_show_alloc_mem(t_mglobal *g);

typedef enum		e_log_types {
	LOGT_MALLOC,
	LOGT_REALLOC,
	LOGT_FREE,
	LOGT_BADFREE,
}					t_log_types;

void				log_call(t_mglobal *g, int which, void *ptr, size_t size);

#endif
