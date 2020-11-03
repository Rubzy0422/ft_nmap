/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display_header.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcoetzer <rcoetzer@student.wethinkcode.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/03 18:51:15 by rcoetzer          #+#    #+#             */
/*   Updated: 2020/11/03 19:07:18 by rcoetzer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_nmap.h>

void display_scans(uint8_t scan_list[SCAN_MAX], uint8_t number_scans)
{
	printf("SCAN TODO:\t");
	for (uint8_t i = 0; i < number_scans; ++i)
	{
		if (scan_list[i] == SYN_FLAG)
			printf("SYN ");
		if (scan_list[i] == NULL_FLAG)
			printf("NULL ");
		if (scan_list[i] == FIN_FLAG)
			printf("FIN ");
		if (scan_list[i] == ACK_FLAG)
			printf("ACK ");
		if (scan_list[i] == XMAS_FLAG)
			printf("XMAS ");
		if (scan_list[i] == UDP_FLAG)
			printf("UDP ");
	}
	printf("\n");
}

void display_header(t_env *env)
{
	printf("[STARTING FT_NMAP]\n\n");
	printf("PORT AMOUNT:\t%d\n", env->params.portcnt);
	printf("HOST AMOUNT:\t%d\n", env->hostcnt);
	printf("THRED AMOUNT:\t%d\n", env->params.thread_cnt);
	display_scans(env->scan_list, env->number_scans);
	printf("\n");
}