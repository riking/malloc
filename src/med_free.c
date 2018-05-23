/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   med_free.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyork <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/23 16:55:39 by kyork             #+#    #+#             */
/*   Updated: 2018/05/23 16:57:01 by kyork            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc_private.h"

ssize_t				med_free(t_region *page, size_t idx)
{
	t_u64	maskval;

	maskval = atomic_load(pg_bitset_ptr(page, idx));
	// TODO
	return (-1);
}
