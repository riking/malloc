/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alloc_way_too_much.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyork <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/29 15:25:14 by kyork             #+#    #+#             */
/*   Updated: 2018/05/30 18:58:36 by kyork            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <malloc.h>
#include <ft_printf.h>

int		main(void)
{
	size_t		size;
	void		*ptr;

	size = (((size_t)1) << 49);
	ptr = malloc(size);
	if (ptr)
		ft_printf("allocation succeeded, ret=%p\n", ptr);
	else
	{
		ft_perrorf("allocation failed, should be ENOMEM");
	}
}
