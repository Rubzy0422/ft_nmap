/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   packet_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcoetzer <rcoetzer@student.wethinkcode.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/13 21:34:20 by rcoetzer          #+#    #+#             */
/*   Updated: 2020/11/23 09:24:08 by rcoetzer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_nmap.h>

uint8_t handle_tcp(struct tcphdr *tcp_header, t_scanres *scan)
{
	if (DEBUG_TCP)
		print_tcpheader(tcp_header);
	if (scan->port != ntohs(tcp_header->source))
		return 0;
	if (scan->scan == SYN_FLAG && ntohs(tcp_header->dest) == SYN_PORT)
	{
		if ((tcp_header->syn && tcp_header->ack) || (tcp_header->syn && !tcp_header->rst))
		{
			scan->state = OPENED_STATE;
			return 1;
		}
		else if (tcp_header->rst)
		{
			scan->state = CLOSED_STATE;
			return 1;
		}	
	}
	if ((scan->scan == NULL_FLAG && ntohs(tcp_header->dest) == NULL_PORT)
	|| (scan->scan == FIN_FLAG && ntohs(tcp_header->dest) == FIN_PORT)
	|| (scan->scan == XMAS_FLAG && ntohs(tcp_header->dest) == XMAS_PORT)) 
	{
		if (tcp_header->rst)
		{
			scan->state = CLOSED_STATE;
			return 1;
		}
	}
	if (scan->scan == ACK_FLAG && ntohs(tcp_header->dest) == ACK_PORT)
	{
		if (tcp_header->ack)
		{
			scan->state = UNFILTERED_STATE;
			return 1;
		}
	}
	return 0;
}

uint8_t handle_udp(struct udphdr *udp_header, t_scanres *scan)
{
	if (DEBUG_UDP)
		if (ntohs(udp_header->uh_dport) != 53) //don't print dns
			print_udpheader(udp_header);
	if (scan->port != ntohs(udp_header->dest))
			return 0;
	if (scan->scan == UDP_FLAG && ntohs(udp_header->source) == UDP_PORT)
	{
		scan->state = OPENED_STATE;
		return 1;
	}
	return 0;
}

int acceptable_code(int flag, int code)
{
	if (flag != UDP_FLAG && (
		code == ICMP_NET_UNREACH || code == ICMP_HOST_UNREACH ||
		code == ICMP_PROT_UNREACH || code == ICMP_PORT_UNREACH ||
		code == ICMP_NET_ANO || code == ICMP_HOST_ANO || 
		code == ICMP_PKT_FILTERED))
		return 1;
	else if (flag == UDP_FLAG && (
		code == ICMP_NET_UNREACH || code == ICMP_HOST_UNREACH ||
		code == ICMP_PROT_UNREACH || code == ICMP_NET_ANO || 
		code == ICMP_HOST_ANO || code == ICMP_PKT_FILTERED))
		return 1;
	return 0;
}

uint8_t handle_icmp(t_icmprespack *icmp, t_scanres *scan)
{
	if (DEBUG_ICMP)
		print_icmpheader(&icmp->icmph);
	if (scan->port != ntohs(icmp->udph.dest))
		return 0;
	if (scan->scan == SYN_FLAG && ntohs(icmp->udph.source) == SYN_PORT)
	{								//3
		if ((icmp->icmph.type == ICMP_DEST_UNREACH) && acceptable_code(SYN_FLAG, icmp->icmph.code))
		{
			scan->state = FILTERED_STATE;
			return 1;
		}
	}
	if ((scan->scan == NULL_FLAG && ntohs(icmp->udph.source) == NULL_PORT )
	|| (scan->scan == FIN_FLAG && ntohs(icmp->udph.source) == FIN_PORT)
	|| (scan->scan == XMAS_FLAG && ntohs(icmp->udph.source) == XMAS_PORT))
	{
		if ((icmp->icmph.type == ICMP_DEST_UNREACH) && 
			acceptable_code(NULL_FLAG, icmp->icmph.code))
		{
			scan->state = FILTERED_STATE;
			return 1;
		}
	}
	if (scan->scan == ACK_FLAG && ntohs(icmp->udph.source) == ACK_PORT)
	{
		if ((icmp->icmph.type == ICMP_DEST_UNREACH) && acceptable_code(ACK_FLAG, icmp->icmph.code))
		{
			scan->state = FILTERED_STATE;
			return 1;
		}
	}
	if (scan->scan == UDP_FLAG && ntohs(icmp->udph.source) == UDP_PORT)
	{
		if ((icmp->icmph.type == ICMP_DEST_UNREACH) && acceptable_code(UDP_FLAG, icmp->icmph.code))
		{
			scan->state = FILTERED_STATE;
			return 1;
		}
		else if (icmp->icmph.type  == ICMP_DEST_UNREACH && icmp->icmph.code == 3)
		{
			scan->state = CLOSED_STATE;
			return 1;
		}
	}
	return 0;
}

void send_scan(t_scanres *scan, in_addr_t source_ip, int s)
{
	if (scan != NULL)
	{
		if (scan->scan == SYN_FLAG)
			send_syn(scan->host, source_ip, s, scan->port);
		else if (scan->scan == NULL_FLAG)
			send_null(scan->host, source_ip, s, scan->port);
		else if (scan->scan == ACK_FLAG)
			send_ack(scan->host, source_ip, s, scan->port);
		else if (scan->scan == FIN_FLAG)
			send_fin(scan->host, source_ip, s, scan->port);
		else if (scan->scan == XMAS_FLAG)
			send_xmas(scan->host, source_ip, s, scan->port);
		else if (scan->scan == UDP_FLAG)
			send_udp(scan->host, source_ip, s, scan->port);
	}
}

u_int8_t ProcessPacket(unsigned char* buffer, int protocall,
					 in_addr_t *source_ip, t_scanres *scan)
{
	struct ip *iphdr;
	struct in_addr tmp;
	u_int8_t belong;

	belong = 0;
	iphdr = (struct ip *)buffer;
	tmp.s_addr = *source_ip;
	
	if (ft_strcmp(inet_ntoa(iphdr->ip_dst), inet_ntoa(tmp)))
		return belong;

	if (ft_strcmp(inet_ntoa(iphdr->ip_src), inet_ntoa(scan->host.sin_addr)))
		return belong;
		
	if (protocall == 0)
		belong = handle_icmp((t_icmprespack *)(buffer + sizeof(struct ip)),
		scan);
	if (protocall == 1)
		belong = handle_udp((struct udphdr *)(buffer + sizeof(struct ip)),
		scan);
	if (protocall == 2)
		belong = handle_tcp((struct tcphdr *)(buffer + sizeof(struct ip)),
		scan);
	return belong;
}