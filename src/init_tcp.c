/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_tcp.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcoetzer <rcoetzer@student.wethinkcode.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/04 13:35:20 by rcoetzer          #+#    #+#             */
/*   Updated: 2020/11/05 18:09:10 by rcoetzer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_nmap.h>

void	tcp_header_forge(t_tcppack *tcp_pack, int src_port, int dest_port, t_tcpflags flags)
{
	tcp_pack->tcph->source = htons (src_port);
	tcp_pack->tcph->dest = htons (dest_port);
	tcp_pack->tcph->seq = 0;
	tcp_pack->tcph->ack_seq = 0;
	tcp_pack->tcph->doff = 5;
	tcp_pack->tcph->fin= flags.fin;	
	tcp_pack->tcph->syn= flags.syn;
	tcp_pack->tcph->rst= flags.rst;
	tcp_pack->tcph->psh= flags.psh;
	tcp_pack->tcph->ack= flags.ack;
	tcp_pack->tcph->urg= flags.urg;
	tcp_pack->tcph->window = htons (1024);	// /* maximum allowed window size */
	tcp_pack->tcph->check = 0;	//leave checksum 0 now, filled later by pseudo header
	tcp_pack->tcph->urg_ptr = 0;
}

void	tcp_ipheader_forge(t_tcppack *tcp_pack,in_addr_t source_ip, int datalen, struct sockaddr_in sin)
{
	//Fill in the IP Header
	tcp_pack->iph->ihl = 5;
	tcp_pack->iph->version = 4;
	tcp_pack->iph->tos = 0;
	tcp_pack->iph->tot_len = sizeof (struct iphdr) + sizeof (struct tcphdr) + datalen;
	tcp_pack->iph->id = htonl (getpid());
	tcp_pack->iph->frag_off = 0;
	tcp_pack->iph->ttl = 255;
	tcp_pack->iph->protocol = IPPROTO_TCP;
	tcp_pack->iph->check = 0;
	tcp_pack->iph->saddr = source_ip;
	tcp_pack->iph->daddr = sin.sin_addr.s_addr;
	tcp_pack->iph->check = csum ((unsigned short *)tcp_pack->datagram, tcp_pack->iph->tot_len);
}

void	tcp_psh_forge(t_tcppack *tcp_pack, in_addr_t source_ip,  int datalen, struct sockaddr_in sin)
{
	tcp_pack->psh.source_address = source_ip;
	tcp_pack->psh.dest_address = sin.sin_addr.s_addr;
	tcp_pack->psh.placeholder = 0;
	tcp_pack->psh.protocol = IPPROTO_TCP;
	tcp_pack->psh.pack_length = htons(sizeof(struct tcphdr) + datalen);
}

void	tcp_complete_forge(t_tcppack *tcp_pack, int datalen)
{
	int psize = sizeof(struct pseudo_header) + sizeof(struct tcphdr) + datalen;
	tcp_pack->pseudogram = malloc(psize);
	
	ft_memcpy(tcp_pack->pseudogram , (char*) &tcp_pack->psh,
	sizeof (struct pseudo_header));
	ft_memcpy(tcp_pack->pseudogram + sizeof(struct pseudo_header), tcp_pack->tcph,
	sizeof(struct tcphdr) + datalen);
	tcp_pack->tcph->check = csum((unsigned short*)tcp_pack->pseudogram, psize);
}

void	init_tcp(t_tcppack *tcp_pack, struct sockaddr_in sin, in_addr_t source_ip,int src_port, int dest_port, t_tcpflags flags)
{
	int datalen;

	ft_bzero(&tcp_pack->datagram, sizeof(tcp_pack->datagram));
	tcp_pack->iph =(struct iphdr *)tcp_pack->datagram;
	tcp_pack->tcph = (struct tcphdr *) (tcp_pack->datagram + sizeof (struct ip));
	tcp_pack->data = tcp_pack->datagram + sizeof(struct iphdr) + sizeof(struct tcphdr);
	ft_strcpy(tcp_pack->data , "");
	datalen = ft_strlen(tcp_pack->data);
	tcp_ipheader_forge(tcp_pack, source_ip, datalen, sin);
	tcp_header_forge(tcp_pack, src_port, dest_port, flags);
	tcp_psh_forge(tcp_pack, source_ip, datalen, sin);
	tcp_complete_forge(tcp_pack, datalen);
}