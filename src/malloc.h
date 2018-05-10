/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyork <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/09 17:08:49 by kyork             #+#    #+#             */
/*   Updated: 2018/05/09 17:11:20 by kyork            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MALLOC42_MALLOC_H
# define MALLOC42_MALLOC_H

# include <stddef.h>

void			free(void *ptr);
void			*malloc(size_t size);
void			*realloc(void *ptr, size_t size);

void			show_alloc_mem(void);

#endif
