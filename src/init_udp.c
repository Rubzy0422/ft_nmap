/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_udp.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcoetzer <rcoetzer@student.wethinkcode.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/04 13:35:20 by rcoetzer          #+#    #+#             */
/*   Updated: 2020/11/05 10:10:14 by rcoetzer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_nmap.h>

void	udp_header_forge(t_udppack *udp_pack, int src_port, int dest_port, int datalen)
{
	udp_pack->udph->source = htons (src_port);
	udp_pack->udph->dest = htons (dest_port);
	udp_pack->udph->len = htons(8 + datalen);	
	udp_pack->udph->check = 0;
}

void	udp_ipheader_forge(t_udppack *udp_pack,in_addr_t source_ip, int datalen, struct sockaddr_in sin)
{
	//Fill in the IP Header
	udp_pack->iph->ihl = 5;
	udp_pack->iph->version = 4;
	udp_pack->iph->tos = 0;
	udp_pack->iph->tot_len = sizeof (struct iphdr) + sizeof (struct udphdr) + datalen;
	udp_pack->iph->id = htonl (getpid());
	udp_pack->iph->frag_off = 0;
	udp_pack->iph->ttl = TTL;
	udp_pack->iph->protocol = IPPROTO_UDP;
	udp_pack->iph->check = 0;
	udp_pack->iph->saddr = source_ip;
	udp_pack->iph->daddr = sin.sin_addr.s_addr;
	udp_pack->iph->check = csum ((unsigned short *)udp_pack->datagram, udp_pack->iph->tot_len);
}

void	udp_psh_forge(t_udppack *udp_pack, in_addr_t source_ip,  int datalen, struct sockaddr_in sin)
{
	udp_pack->psh.source_address = source_ip;
	udp_pack->psh.dest_address = sin.sin_addr.s_addr;
	udp_pack->psh.placeholder = 0;
	udp_pack->psh.protocol = IPPROTO_UDP;
	udp_pack->psh.pack_length = htons(sizeof(struct udphdr) + datalen);
}

void	udp_complete_forge(t_udppack *udp_pack, int datalen)
{
	int psize = sizeof(struct pseudo_header) + sizeof(struct udphdr) + datalen;
	udp_pack->pseudogram = malloc(psize);
	
	memcpy(udp_pack->pseudogram , (char*) &udp_pack->psh,
	sizeof (struct pseudo_header));
	memcpy(udp_pack->pseudogram + sizeof(struct pseudo_header), udp_pack->udph,
	sizeof(struct udphdr) + datalen);
	udp_pack->udph->check = csum((unsigned short*)udp_pack->pseudogram, psize);
}

void	init_udp(t_udppack *udp_pack, struct sockaddr_in sin, in_addr_t source_ip,int src_port, int dest_port)
{
	int datalen;

	sin.sin_port = htons(dest_port);
	ft_bzero(&udp_pack->datagram, sizeof(udp_pack->datagram));
	udp_pack->iph =(struct iphdr *)udp_pack->datagram;
	udp_pack->udph = (struct udphdr *) (udp_pack->datagram + sizeof (struct ip));
	udp_pack->data = udp_pack->datagram + sizeof(struct iphdr) + sizeof(struct udphdr);
	ft_strcpy(udp_pack->data , "");
	datalen = strlen(udp_pack->data);

	udp_ipheader_forge(udp_pack, source_ip, datalen, sin);
	udp_header_forge(udp_pack, src_port, dest_port, datalen);
	udp_psh_forge(udp_pack, source_ip, datalen, sin);
	udp_complete_forge(udp_pack, datalen);
}