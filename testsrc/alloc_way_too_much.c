/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alloc_way_too_much.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyork <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/29 15:25:14 by kyork             #+#    #+#             */
/*   Updated: 2018/05/29 15:29:41 by kyork            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <malloc.h>
#include <ft_printf.h>

int		main(void)
{
	size_t		size = (((size_t)1) << 49);
	void		*ptr;

	ft_printf("Printf test: %.40d", 1234);
	return (0);
	ptr = malloc(size);
	if (ptr)
		ft_printf("allocation succeeded, ret=%p\n", ptr);
	else
	{
		ft_perrorf("allocation failed, should be ENOMEM");
	}
}
