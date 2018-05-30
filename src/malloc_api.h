/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc_api.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyork <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/09 17:46:31 by kyork             #+#    #+#             */
/*   Updated: 2018/05/30 13:58:40 by kyork            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MALLOC_API_H
# define MALLOC_API_H

# define EXPORT_VOIDSTAR void* __attribute__((visibility("default")))
# define EXPORT_VOID void __attribute__((visibility("default")))
# define PRIVATE_VOID void __attribute__((visibility("hidden")))

# include <stddef.h>

extern t_mglobal		g_mglobal;

PRIVATE_VOID			malloc_setup_stub(void);
EXPORT_VOIDSTAR			malloc(size_t size);
EXPORT_VOIDSTAR			calloc(size_t a, size_t b);
EXPORT_VOIDSTAR			valloc(size_t size);
EXPORT_VOID				free(void *ptr);
EXPORT_VOIDSTAR			realloc(void *ptr, size_t size);
EXPORT_VOIDSTAR			reallocf(void *ptr, size_t size);
EXPORT_VOID				show_alloc_mem(void);
EXPORT_VOID				show_alloc_mem_ex(void);

#endif
