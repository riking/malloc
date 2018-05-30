/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyork <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/09 17:08:49 by kyork             #+#    #+#             */
/*   Updated: 2018/05/30 13:58:34 by kyork            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MALLOC42_MALLOC_H
# define MALLOC42_MALLOC_H

# include <stddef.h>

void			free(void *ptr);
void			*malloc(size_t size);
void			*calloc(size_t size, size_t count);
void			*valloc(size_t size);
void			*realloc(void *ptr, size_t size);
void			*reallocf(void *ptr, size_t size);

void			show_alloc_mem(void);
void			show_alloc_mem_ex(void);

#endif
