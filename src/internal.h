/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   internal.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyork <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/09 17:46:31 by kyork             #+#    #+#             */
/*   Updated: 2018/05/09 17:48:28 by kyork            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MALLOC42_INTERNAL_H
# define MALLOC42_INTERNAL_H

# define EXPORT(type) type __attribute__((visibility("default")))

# include <stddef.h>

EXPORT(void *)				malloc(size_t size);
EXPORT(void)				free(void *ptr);
EXPORT(void *)				realloc(void *ptr, size_t size);
EXPORT(void)				show_alloc_mem(void);

#endif
