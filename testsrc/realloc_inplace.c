/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   realloc_inplace.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyork <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/30 10:09:09 by kyork             #+#    #+#             */
/*   Updated: 2018/05/30 10:18:18 by kyork            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <ft_printf.h>

int		main(void)
{
	char	*ptr1;
	char	*ptr2;

	ptr1 = malloc(3);
	ptr1[0] = 'a';
	ptr1[1] = 0;
	free(ptr1);
	ptr2 = realloc(ptr1, 7);
	ptr2[1] = 'b';
	ptr2[2] = 0;
	if (ptr1 == ptr2)
		ft_printf("OK 3-7 in place %s %p %p\n", ptr2, ptr1, ptr2);
	else
		ft_printf("BAD 3-7 moved: %s %p %p\n", ptr2, ptr1, ptr2);
	ptr1 = realloc(ptr2, 11);
	ptr1[2] = 'c';
	ptr1[3] = 0;
	if (ptr1 == ptr2)
		ft_printf("BAD 7-11 in place %s %p %p\n", ptr1, ptr1, ptr2);
	else
		ft_printf("OK 7-11 moved: %s %p %p\n", ptr1, ptr1, ptr2);
	ptr2 = realloc(ptr1, 64);
	ptr2[3] = 'c';
	ptr2[4] = 0;
	if (ptr1 == ptr2)
		ft_printf("OK 11-64 in place %s %p %p\n", ptr2, ptr1, ptr2);
	else
		ft_printf("BAD 11-64 moved: %s %p %p\n", ptr2, ptr1, ptr2);
}
