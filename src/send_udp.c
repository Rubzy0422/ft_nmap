/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   send_udp.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcoetzer <rcoetzer@student.wethinkcode.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/05 10:03:14 by rcoetzer          #+#    #+#             */
/*   Updated: 2020/11/16 14:30:24 by rcoetzer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_nmap.h>

void send_udp(struct sockaddr_in sin, in_addr_t source_ip, int sock, int portnum)
{
	t_udppack			*udp_pack;
	int					send;

	udp_pack = malloc(sizeof(t_udppack));
	if (!udp_pack)
		ft_error("Could not malloc for udp_pack!", EXIT_FAILURE);
	ft_bzero(udp_pack, sizeof(udp_pack));
	sin.sin_port = htons(portnum);
	sin.sin_family = AF_INET;
	init_udp(udp_pack, sin, source_ip, UDP_PORT, portnum);
	send = sendto(sock, udp_pack->datagram, udp_pack->iph->tot_len, 0,
		(struct sockaddr *)&sin, sizeof(sin));
	if (send < 0)
		ft_error("sendto failed", send);
	ft_free(udp_pack->pseudogram);
	ft_free(udp_pack);
}