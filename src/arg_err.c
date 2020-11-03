/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arg_err.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcoetzer <rcoetzer@student.wethinkcode.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/03 13:10:19 by rcoetzer          #+#    #+#             */
/*   Updated: 2020/11/03 13:11:47 by rcoetzer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_nmap.h>

void arg_err(char *argname)
{
	printf("ft_namp option %s requires an argument\n", argname);
	printf("See the output of ft_nmap --help for a summary of options.\n");
	exit(EXIT_FAILURE);
}