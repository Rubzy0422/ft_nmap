/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   send_null.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcoetzer <rcoetzer@student.wethinkcode.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/04 14:14:55 by rcoetzer          #+#    #+#             */
/*   Updated: 2020/11/10 22:02:12 by rcoetzer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_nmap.h>

void send_null(struct sockaddr_in sin, in_addr_t source_ip, int sock, int portnum)
{
	t_tcppack			*tcp_pack;
	t_tcpflags			flags;
	int					send;

	tcp_pack = malloc(sizeof(t_tcppack));
	if (!tcp_pack)
		ft_error("Could not malloc for tcp_pack!", EXIT_FAILURE);
	
	ft_bzero(&flags, sizeof(flags));
	ft_bzero(tcp_pack, sizeof(tcp_pack));
	sin.sin_port = htons(portnum);
	sin.sin_family = AF_INET;
	init_tcp(tcp_pack, sin, source_ip, NULL_PORT, portnum, flags);
	send = sendto(sock, tcp_pack->datagram, tcp_pack->iph->tot_len, 0,
		(struct sockaddr *)&sin, sizeof(sin));
	if (send < 0)
		ft_error("sendto failed", send);
	ft_free(tcp_pack->pseudogram);
	ft_free(tcp_pack);
}