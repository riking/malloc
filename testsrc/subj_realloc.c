/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   subj_realloc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyork <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/30 14:31:52 by kyork             #+#    #+#             */
/*   Updated: 2018/05/30 19:01:04 by kyork            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#define M (1024 * 1024)

static void		print(char *s)
{
	write(1, s, strlen(s));
}

int				main(void)
{
	char *addr1;
	char *addr3;

	addr1 = (char*)malloc(16 * M);
	strcpy(addr1, "Bonjour\n");
	print(addr1);
	(void)malloc(16 * M);
	addr3 = (char*)realloc(addr1, 128 * M);
	addr3[127 * M] = 42;
	print(addr3);
	return (0);
}
