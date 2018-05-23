/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pages.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyork <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/23 14:16:28 by kyork             #+#    #+#             */
/*   Updated: 2018/05/23 14:27:03 by kyork            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MALLOC_PAGES_H
# define MALLOC_PAGES_H

# include "malloc_private.h"

typedef struct		s_pagesizes {
	t_size_class	item_class;
	size_t			size;
	t_u16			item_count;
	t_u16			bitset_bytes;
}					t_pagesizes;

#endif
