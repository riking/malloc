/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simple_with_free.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyork <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/30 09:06:32 by kyork             #+#    #+#             */
/*   Updated: 2018/05/30 10:01:14 by kyork            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <malloc.h>

extern void	show_alloc_mem(void);

int		main(void)
{
	int		i;
	char	*addr;

	i = 0;
	while (i < 1024)
	{
		addr = (char*)malloc(1024);
		addr[0] = 42;
		addr[1023] = 41;
		free(addr);
		i++;
	}
	show_alloc_mem();
	return (0);
}
