/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_print.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcoetzer <rcoetzer@student.wethinkcode.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/19 10:49:21 by rcoetzer          #+#    #+#             */
/*   Updated: 2020/11/19 20:49:51 by rcoetzer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_nmap.h>

void print_icmpheader(struct icmphdr * icmp_header)
{
	char *icmp_out;
	asprintf(&icmp_out,"==== ICMP ====\nCHECKSUM : %d\t\nCODE : \t\t%d\nTYPE : \t\t%d\nECHO ID :\t%d\nECHO SEQ : \t%d\nGATEWAY : \t%d\n\n\n",
	icmp_header->checksum,
	icmp_header->code,
	icmp_header->type,
	icmp_header->un.echo.id,
	icmp_header->un.echo.sequence,
	icmp_header->un.gateway);
	puts(icmp_out);
}

void print_tcpheader(struct tcphdr *tcph)
{
	char *tcp_out;
	asprintf(&tcp_out, "==== TCP ====\nSource Port:\t%d\nDest Port:\t\t%d\ntcp SEQ:\t\t%d\nACK_SEQ:\t\t%d\nDOFF:\t\t\t%d\nFIN FLAG:\t\t%d\nSYN FLAG:\t\t%d\nRST FLAG:\t\t%d\nPSH FLAG:\t\t%d\nACK FLAG:\t\t%d\nURG FLAG:\t\t%d\nWINDOW:\t\t\t%d\nCHECK:\t\t\t%d\nURG PTR:\t\t%d\n\n\n",
	ntohs(tcph->source),
	ntohs(tcph->dest),
	tcph->seq,
	tcph->ack_seq,
	tcph->doff,
	tcph->fin,	
	tcph->syn,
	tcph->rst,
	tcph->psh,
	tcph->ack,
	tcph->urg,
	ntohs(tcph->window),
	tcph->check,
	tcph->urg_ptr);

	puts(tcp_out);
	return ; 
}

void print_udpheader(struct udphdr *udph)
{
	char *udp_out;

	asprintf(&udp_out, "==== UDP ====\nCHECK : %d\t\nDEST : %d\t\nLEN : %d\t\nSOURCE : %d\t\nUH DPORT : %d\t\nUH SPORT : %d\t\nUH SUM : %d\t\nUH LEN: %d\t\n\n", 
	udph->check, 
	udph->dest,
	udph->len,
	udph->source,
	ntohs(udph->uh_dport),
	ntohs(udph->uh_sport),
	udph->uh_sum,
	udph->uh_ulen);
	puts(udp_out);
}