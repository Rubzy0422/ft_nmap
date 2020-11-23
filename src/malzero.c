/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malzero.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcoetzer <rcoetzer@student.wethinkcode.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/19 18:53:12 by rcoetzer          #+#    #+#             */
/*   Updated: 2020/11/19 18:56:24 by rcoetzer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_nmap.h>

void *malzero(size_t size)
{
	void *ptr = malloc(size);
	if (!ptr)
		return NULL;
	ft_bzero(ptr, size);
	return ptr;
}