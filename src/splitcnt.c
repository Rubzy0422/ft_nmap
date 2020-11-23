/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   splitcnt.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcoetzer <rcoetzer@student.wethinkcode.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/23 08:46:14 by rcoetzer          #+#    #+#             */
/*   Updated: 2020/11/23 09:05:01 by rcoetzer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_nmap.h>

int splitcnt(char **split)
{
	int i = 0;

	if (!split)
		return 0;
	while(split[i])
		i++;
	return i;
}