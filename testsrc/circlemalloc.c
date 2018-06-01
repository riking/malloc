/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   circlemalloc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyork <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/23 17:27:51 by kyork             #+#    #+#             */
/*   Updated: 2018/05/31 18:19:58 by kyork            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <malloc.h>

#include <stdlib.h>
#include <stdio.h>

int			main(int argc, char **argv)
{
	int		size;
	void	*buffer[5000];
	int		idx;
	int		off;

	if (argc == 2)
		size = ft_atoi(argv[1]);
	else
		size = 8;
	idx = 0;
	while (idx < 5000)
		buffer[idx++] = 0;
	idx = 0;
	while (idx < 5000 * 2)
	{
		off = (idx + rand()) % 5000;
		free(buffer[off]);
		buffer[off] = NULL;
		free(buffer[idx % 5000]);
		buffer[idx % 5000] = malloc(size);
		idx++;
	}
	printf("Done\n");
}
