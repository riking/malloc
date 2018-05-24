/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simple.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyork <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/23 15:13:04 by kyork             #+#    #+#             */
/*   Updated: 2018/05/23 17:26:55 by kyork            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <libft.h>

#define DATA_T int

void			show_alloc_mem(void);

static void		deldata(void *ptr, size_t size)
{
	if (size != sizeof(DATA_T))
		abort();
	free(ptr);
}

int				main(void)
{
	DATA_T	data;
	t_list	*head;
	t_list	*tmphead;

	data = 1;
	head = ft_lstnew(&data, sizeof(data));
	data = 2;
	ft_lstadd(&head, ft_lstnew(&data, sizeof(data)));
	data = 3;
	ft_lstadd(&head, ft_lstnew(&data, sizeof(data)));
	data = 3;
	tmphead = head;
	head = head->next;
	ft_lstdelone(&tmphead, deldata);
	ft_lstdel(&head, deldata);
	show_alloc_mem();
}
