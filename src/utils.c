/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcoetzer <rcoetzer@student.wethinkcode.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/22 14:56:03 by rcoetzer          #+#    #+#             */
/*   Updated: 2020/11/07 14:55:24 by rcoetzer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_nmap.h>

uint64_t	char_count(char *str, char c)
{
	int x;
	uint64_t cnt;
	
	x = 0;
	cnt = 0;
	if (!str || !c)
		return cnt;
	while (str[x])
	{
		if (str[x] == c)
			cnt++;
		x++;
	}
	return cnt;
}

char	*ft_strtoupper(char *str)
{
	int i;

	i = -1;
	if (!str)
		return NULL;
	while (str[++i])
		str[i] = ft_toupper(str[i]);
	return str;
}

char	*ft_strtolower(char *str)
{
	int i;
	i = -1;
	if (!str)
		return NULL;
	while (str[++i])
		str[i] = ft_tolower(str[i]);
	return str;
}

int in_arr(int val, uint8_t *arr, int arrlen)
{
	uint8_t i;

	if (!arr)
		return 0;
	for(i = 0; i < arrlen; i++)
	{
		if(arr[i] == val)
		return 1;
	}
	return 0;
}