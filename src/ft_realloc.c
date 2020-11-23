/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_realloc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcoetzer <rcoetzer@student.wethinkcode.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/19 12:53:31 by rcoetzer          #+#    #+#             */
/*   Updated: 2020/11/19 12:57:41 by rcoetzer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_nmap.h>

void *ft_realloc(void *ptr, size_t size)
{
	void *new;
	
	new = malloc(size);
	if (!new)
		return ptr;
	ft_memcpy(new, ptr, size);
	ft_free(ptr);
	return new;
}