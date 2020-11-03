/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display_help.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcoetzer <rcoetzer@student.wethinkcode.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/03 12:31:39 by rcoetzer          #+#    #+#             */
/*   Updated: 2020/11/03 12:33:26 by rcoetzer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_nmap.h>

void	display_help(int32_t exitcode)
{
	printf("Help Screen\n");
	printf("ft_nmap [OPTIONS]\n");
	printf("--help\t\t");
	printf("Print this help screen\n");
	printf("--ports\t\t");
	printf("ports to scan (eg: 1-10 or 1,2,3 or 1,5-15)\n");
	printf("--ip\t\t");
	printf("ip addresses to scan in dot format\n");
	printf("--file\t\t");
	printf("File name containing IP addresses to scan,\n");
	printf("--speedup\t");
	printf("[250 max] number of parallel threads to use\n");
	printf("--scan\t\t");
	printf("SYN/NULL/FIN/XMAS/ACK/UDP\n");
	exit(exitcode);
}