/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   double_free.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyork <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/29 15:22:26 by kyork             #+#    #+#             */
/*   Updated: 2018/05/30 14:24:43 by kyork            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <malloc.h>

int		main(void)
{
	char	*ptr;

	ptr = malloc(3);
	free(ptr);
	printf("should complain about double free %p:\n", ptr);
	free(ptr);
	ptr = malloc(30);
	free(ptr);
	printf("should complain about double free %p:\n", ptr);
	free(ptr);
	ptr = malloc(900);
	free(ptr);
	printf("should complain about double free %p:\n", ptr);
	free(ptr);
	ptr = malloc(90000);
	free(ptr);
	printf("should complain about double free %p:\n", ptr);
	free(ptr);
}
