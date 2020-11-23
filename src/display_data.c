/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display_data.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcoetzer <rcoetzer@student.wethinkcode.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/19 11:34:59 by rcoetzer          #+#    #+#             */
/*   Updated: 2020/11/23 08:05:19 by rcoetzer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_nmap.h>

void display_scan_type(uint8_t scan)
{
	if (scan == SYN_FLAG)
		printf("\tSYN");
	if (scan == NULL_FLAG)
		printf("\tNULL");
	if (scan == FIN_FLAG)
		printf("\tFIN");
	if (scan == XMAS_FLAG)
		printf("\tXMAS");
	if (scan == UDP_FLAG)
		printf("\tUDP");
	if (scan == ACK_FLAG)
		printf("\tACK");
}

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
	uint32_t pport = MAX_PORTS + 1;
	char *phost = NULL;
	char *chost = NULL;
	int recv;

	gettimeofday(&tstop, NULL);
	float sub = time_sub(&tstop,&tstart);
	while (i < scannum)
	{
		if (chost)
			ft_free(phost);
		phost = chost;
		chost = ft_strdup(inet_ntoa(results[i].host.sin_addr));
		
		if (!phost || ft_strcmp(chost, phost)) // prtint host
		{
			printf("\033[1;36m %s \033[0m \n", chost);
		}	
		if (results[i].port != pport || ft_strcmp(chost, phost)) //if port change or host change
			display_port(results[i].port);
		pport = results[i].port; 

		if (results[i].scan == params->scan_list[0])
			recv = 0;
		if (results[i].resp && results[i].state != CLOSED_STATE)
		{
			recv = 1;
			printf("\033[0;32m");
		}
		display_scan_type(results[i].scan);
		display_state(results[i].state);
		printf("\033[0m ");
		if (i < scannum && results[i].scan == params->scan_list[params->scancnt - 1])
		{
			if (recv == 1)
				printf("\t\t\t\t \033[0;32m OPEN \033[0m \n");
			else 
				printf("\t\t\t\tCLOSED\n");
		}
		i++;
	}
	ft_free(chost);
	ft_free(phost);
	printf("\n%d Scans took %f Seconds\n", scannum, (sub) * 0.001);
	ft_free(results);
}
