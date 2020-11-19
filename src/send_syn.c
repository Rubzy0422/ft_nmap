/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   send_syn.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcoetzer <rcoetzer@student.wethinkcode.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/04 13:46:46 by rcoetzer          #+#    #+#             */
/*   Updated: 2020/11/14 16:27:58 by rcoetzer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_nmap.h>

void send_syn(struct sockaddr_in sin, in_addr_t source_ip, int sock, int portnum)
{
	t_tcppack			*tcp_pack;
	t_tcpflags			flags;
	int					send;

	tcp_pack = malloc(sizeof(t_tcppack));
	if (!tcp_pack)
		ft_error("Could not malloc for tcp_pack!", EXIT_FAILURE);
	
	ft_bzero(&flags, sizeof(flags));
	ft_bzero(tcp_pack, sizeof(tcp_pack));
	flags.syn = 1;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(portnum);
	init_tcp(tcp_pack, sin, source_ip, SYN_PORT, portnum, flags);
	send = sendto(sock, tcp_pack->datagram, tcp_pack->iph->tot_len, 0,
		(struct sockaddr *)&sin, sizeof(sin));
	if (send < 0)
		ft_error("sendto failed", send);
	ft_free(tcp_pack->pseudogram);
	ft_free(tcp_pack);
}