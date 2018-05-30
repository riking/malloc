/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc_private.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyork <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/24 11:58:13 by kyork             #+#    #+#             */
/*   Updated: 2018/05/30 14:09:53 by kyork            ###   ########.fr       */
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
# define ATOM_S16 _Atomic t_s16
# define VOID_NORETURN void __attribute__((noreturn))

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
** alloc_count - number of live allocations in the region
** item_count - for TINY/MEDIUM, the number of available slots
**              for HUGE, the number of bytes originally requested
** The minimum value for bitset_bytes is 16 (128 bits), as zones must allow
** for at least 100 allocations.
**
** sz_medium gets two bits per slot, using a 0x3 for alloc start and 0x2 for
** continuations
*/

typedef struct		s_region {
	size_t				size;
	void				*page;
	size_t				item_count;
	t_s16				item_class;
	ATOM_S16			alloc_count;
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

	pthread_mutex_t		print_lock;
	pthread_once_t		init_once;
	volatile bool		init_done;

	int					show_temp;
	bool				enable_logging;

	size_t				pagesize;
}					t_mglobal;

void				malloc_setup(t_mglobal *g);
VOID_NORETURN		malloc_panic(const char *str);
VOID_NORETURN		malloc_panicf(const char *fmt, ...);

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
size_t				pg_alloc_idx(t_region *page, void *ptr);
ATOM_U64			*pg_bitset_ptr(t_region *page, size_t idx);

t_size_class		get_size_class(size_t request);

void				*do_malloc(t_mglobal *g, size_t size);
void				*do_calloc(t_mglobal *g, size_t size);

void				*small_malloc(t_mglobal *g, t_size_class cls);
ssize_t				small_free(t_region *page, size_t idx);
size_t				small_show(t_mglobal *g, t_region *page, int flags);

void				*med_malloc(t_mglobal *g, size_t size);
ssize_t				med_getsize(t_region *page, void *ptr);
ssize_t				med_free(t_region *page, size_t idx);
size_t				med_show(t_mglobal *g, t_region *page, int flags);

void				*huge_malloc(t_mglobal *g, size_t size);
ssize_t				huge_free(t_region *page, void *ptr);
size_t				huge_show(t_mglobal *g, t_region *page, int flags);

ssize_t				do_free(t_mglobal *g, void *ptr);
void				*do_realloc(t_mglobal *g, void *ptr, size_t newsize);
void				*do_reallocf(t_mglobal *g, void *ptr, size_t newsize);
t_region			*find_region(t_mglobal *g, char *ptr);
void				try_pagefree(t_mglobal *g, ssize_t page_idx);

# define SHOW_ALLOCD (1 << 0)
# define SHOW_ISFREE (1 << 1)
# define SHOW_TOTAL (1 << 2)
# define SHOW_ZONEHDR (1 << 3)
# define SHOW_SMLZONE (1 << 4)
# define SHOW_MEDZONE (1 << 5)
# define SHOW_LRGZONE (1 << 6)
# define SHOW_CMPLX (1 << 7)

void				show_alloc(t_mglobal *g, int flags,
						void *ptr_start, void *ptr_end);
void				show_ex(t_mglobal *g, int flags,
						void *ptr_start, void *ptr_end);
size_t				do_show_alloc_mem(t_mglobal *g, int flags);

typedef enum		e_log_types {
	LOGT_MALLOC,
	LOGT_FREE,
	LOGT_BADFREE,
	LOGT_REALLOC_IP,
	LOGT_BADREALLOC,
	LOGT_REALLOC_OLD,
	LOGT_REALLOC_NEW,
}					t_log_types;

void				log_call(t_mglobal *g, int which, void *ptr, size_t size);
void				log_callb(t_mglobal *g, int whic, void *ptr, size_t size);

#endif
