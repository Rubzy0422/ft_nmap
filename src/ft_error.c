/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_error.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcoetzer <rcoetzer@student.wethinkcode.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/03 12:22:38 by rcoetzer          #+#    #+#             */
/*   Updated: 2020/11/19 11:02:57 by rcoetzer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_nmap.h>

void ft_error(char *msg, int errcode)
{
	if (errcode == EXIT_FAILURE)
		printf("%s\n", msg);
	else
	{
		perror(msg);
		printf("\tERRCODE: %d\n", errcode);
	}
	exit(errcode);
}
