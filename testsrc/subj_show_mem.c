/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   subj_show_mem.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyork <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/30 14:35:43 by kyork             #+#    #+#             */
/*   Updated: 2018/05/30 14:36:23 by kyork            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <malloc.h>

int		main(void)
{
	malloc(1024);
	malloc(1024 * 32);
	malloc(1024 * 1024);
	malloc(1024 * 1024 * 16);
	malloc(1024 * 1024 * 128);
	show_alloc_mem();
	return (0);
}