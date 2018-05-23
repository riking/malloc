/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc_api.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyork <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/09 17:46:31 by kyork             #+#    #+#             */
/*   Updated: 2018/05/23 12:41:42 by kyork            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MALLOC_API_H
# define MALLOC_API_H

# define EXPORT(type) type __attribute__((visibility("default")))

# include <stddef.h>

EXPORT(void *)				malloc(size_t size);
EXPORT(void)				free(void *ptr);
EXPORT(void *)				realloc(void *ptr, size_t size);
EXPORT(void)				show_alloc_mem(void);

#endif
