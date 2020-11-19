/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   send_recv_pack.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcoetzer <rcoetzer@student.wethinkcode.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/19 11:31:21 by rcoetzer          #+#    #+#             */
/*   Updated: 2020/11/19 12:42:54 by rcoetzer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_nmap.h>

void *send_recv_pack(void *scan_mem)
{
	int one = 1;
	int ssock = socket(AF_INET , SOCK_RAW | SOCK_NONBLOCK , IPPROTO_RAW);
	if(ssock < 0)
		ft_error("Could not create socket", ssock);
	if (setsockopt (ssock, IPPROTO_IP, IP_HDRINCL, &one, sizeof (one)) < 0)
		ft_error("Error setting IP_HDRINCL" , EXIT_FAILURE);
	
	//RECV DATA
	t_threaddata *data;
	int data_size;
	t_scanres *scan;
	struct timeval st;
	
	unsigned char *buffer;

	data = malloc(sizeof(t_threaddata));
	buffer = (unsigned char *)malloc(8192);
	scan = malloc(sizeof(t_scanres));
	if (!data || !buffer || !scan)
		ft_error("Could not malloc to do recv", EXIT_FAILURE);
	
	ft_memcpy(data, (t_threaddata *)scan_mem, sizeof(t_threaddata));
	data->lock = ((t_threaddata *)scan_mem)->lock; // manually set lock lol 
	ft_memcpy(scan, &data->scan, sizeof(t_scanres));
	pthread_mutex_unlock(data->lock);

	int rsock_icmp = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	int rspck_udp = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
	int rsock_tcp = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);

	struct pollfd poll_set[3]; 
	int time = (MSTIMEOUT/RETRIES);

	poll_set[0].fd = rsock_icmp;
	poll_set[0].events = POLLIN | POLLERR;
	poll_set[1].fd = rspck_udp;
	poll_set[1].events = POLLIN | POLLERR;
	poll_set[2].fd = rsock_tcp;
	poll_set[2].events = POLLIN | POLLERR; 

	gettimeofday(&st, NULL);
	int result;
	send_scan(scan, data->source_ip, ssock);
	while(1)
	{
		result = poll(poll_set, 3, time);
		if (result == 0)
		// Well ok then let's just send stuff
		send_scan(scan, data->source_ip, ssock);
		else if (result > 0) 
		{
			for (int i = 0; i < 3; i++)
			{
				if (poll_set[i].revents &POLLIN)
				{
					data_size = recv(poll_set[i].fd , buffer , 8192 , 0);
					if(data_size < 0)
					{
						printf("Recvfrom error , failed to get packets\n");
						break;
					}
					//if I have gotten the data I needed :)
					if (ProcessPacket(buffer, i, &data->source_ip, scan))
					{
						scan->resp = 1;
						break;
					}
				}
			}	
		}
		if (timeout(st))
			break;
	}
	close(rsock_icmp);
	close(rspck_udp);
	close(rsock_tcp);
	close(ssock);
	ft_free(buffer);
	ft_free(data);
	pthread_exit(scan);
}