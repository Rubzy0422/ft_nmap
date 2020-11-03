/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_error.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcoetzer <rcoetzer@student.wethinkcode.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/03 12:22:38 by rcoetzer          #+#    #+#             */
/*   Updated: 2020/11/03 12:34:19 by rcoetzer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_nmap.h>

void ft_error(char *msg, int errcode)
{
	if (errcode == EXIT_FAILURE)
		printf("%s%s%s\n",RED, msg, RESET);
	else
	{
		printf("%s%s", RED, msg);
		perror("");
		printf("ERRCODE: %d\n", errcode);
		printf("%s", RESET);
	}
	exit(errcode);
}
