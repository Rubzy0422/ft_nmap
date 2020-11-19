/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display_data.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcoetzer <rcoetzer@student.wethinkcode.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/19 11:34:59 by rcoetzer          #+#    #+#             */
/*   Updated: 2020/11/19 11:35:20 by rcoetzer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_nmap.h>

void display_state(uint8_t state)
{
	if (state == CLOSED_STATE)
		printf("\tClosed\n");
	else if (state == OPENED_STATE)
		printf("\tOpen\n");
	else if (state == FILTERED_STATE)
		printf("\tFiltered\n");
	else if (state == UNFILTERED_STATE)
		printf("\tUnfiltered\n");
	else if (state == OF_STATE)
		printf("\tOpen|Filtered\n");
	else 
		printf("\tUNKNOWN STATE\n");
}

void display_port(uint32_t port)
{
	struct servent *service;
	service = getservbyport(htons(port), NULL);
	if (service == NULL)
		printf("\n%d\tunknown\n",port);
	else
		printf("\n%d\t%s\n",port, service->s_name);
	return;
}

void display_data(t_scanres *results, int scannum, t_params *params, struct timeval tstart)
{
	struct timeval tstop;
	int i = 0;
	char *s_ip = "";
	char *p_ip = "";
	uint32_t p_port = MAX_PORTS + 1;
	int scan = 0;

	gettimeofday(&tstop, NULL);
	float sub = time_sub(&tstop,&tstart);
	while (i < scannum)
	{
		s_ip = inet_ntoa(results[i].host.sin_addr);
		if (ft_strcmp(s_ip, p_ip))
			printf("HOST:\t %s\n", s_ip);
		p_ip = s_ip;
		if (results[i].port != p_port)
			display_port(results[i].port);
		p_port = results[i].port;
		if (params->scan_list[scan] == SYN_FLAG)
		{
			printf("\tSYN");
			display_state(results[i].state);
		}
		if (params->scan_list[scan] == NULL_FLAG)
		{
			printf("\tNULL");
			display_state(results[i].state);
		}
		if (params->scan_list[scan] == FIN_FLAG)
		{
			printf("\tFIN");
			display_state(results[i].state);
		}
		if (params->scan_list[scan] == ACK_FLAG)
		{
			printf("\tACK");
			display_state(results[i].state);
		}
		if (params->scan_list[scan] == XMAS_FLAG)
		{
			printf("\tXMAS");
			display_state(results[i].state);
		}
		if (params->scan_list[scan] == UDP_FLAG)
		{
			printf("\tUDP");
			display_state(results[i].state);
		}
		scan++;
		if (scan >= params->scancnt)
			scan = 0;
		i++;
	}
	printf("SCAN TOOK %f Seconds\n", (sub) * 0.001);
	ft_free(results);
}
