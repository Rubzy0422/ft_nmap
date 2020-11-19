/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_print.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcoetzer <rcoetzer@student.wethinkcode.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/19 10:49:21 by rcoetzer          #+#    #+#             */
/*   Updated: 2020/11/19 11:38:26 by rcoetzer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_nmap.h>

void print_icmpheader(struct icmphdr * icmp_header)
{
	printf("CHECKSUM : %d\t\n", icmp_header->checksum);
	printf("CODE : \t\t%d\n", icmp_header->code);
	printf("TYPE : \t\t%d\n", icmp_header->type);
	printf("ECHO ID :\t%d\n", icmp_header->un.echo.id);
	printf("ECHO SEQ : \t%d\n", icmp_header->un.echo.sequence);
	printf("GATEWAY : \t%d\n", icmp_header->un.gateway);
	printf("\n\n");
}

void print_tcpheader(struct tcphdr *tcph)
{
	printf("Source Port:\t\t%d\n", ntohs(tcph->source));
	printf("Dest Port:\t\t%d\n", ntohs(tcph->dest));
	printf("tcp SEQ:\t\t%d\n", tcph->seq);
	printf("ACK_SEQ:\t\t%d\n", tcph->ack_seq);
	printf("DOFF:\t\t\t%d\n", tcph->doff);
	printf("FIN FLAG:\t\t%d\n", tcph->fin);	
	printf("SYN FLAG:\t\t%d\n", tcph->syn);
	printf("RST FLAG:\t\t%d\n", tcph->rst);
	printf("PSH FLAG:\t\t%d\n", tcph->psh);
	printf("ACK FLAG:\t\t%d\n", tcph->ack);
	printf("URG FLAG:\t\t%d\n", tcph->urg);
	printf("WINDOW:\t\t\t%d\n", ntohs(tcph->window));
	printf("CHECK:\t\t\t%d\n", tcph->check);
	printf("URG PTR:\t\t%d\n", tcph->urg_ptr);
	printf("\n\n");
	return ; 
}

void print_udpheader(struct udphdr *udph)
{
	printf("CHECK : %d\t\n", udph->check);
	printf("DEST : %d\t\n", udph->dest);
	printf("LEN : %d\t\n", udph->len);
	printf("SOURCE : %d\t\n", udph->source);
	printf("SOURCE : %d\t\n", udph->source);
	printf("UH DPORT : %d\t\n", udph->uh_dport);
	printf("UH SPORT : %d\t\n", udph->uh_sport);
	printf("UH SUM : %d\t\n", udph->uh_sum);
	printf("UH LEN: %d\t\n", udph->uh_ulen);
	printf("\n\n");
}