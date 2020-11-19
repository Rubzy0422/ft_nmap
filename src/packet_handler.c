/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   packet_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcoetzer <rcoetzer@student.wethinkcode.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/13 21:34:20 by rcoetzer          #+#    #+#             */
/*   Updated: 2020/11/19 11:37:46 by rcoetzer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_nmap.h>

uint8_t handle_tcp(struct tcphdr *tcp_header, t_scanres *scan)
{
	if (DEBUG_TCP)
		print_tcpheader(tcp_header);
	if (ntohs(tcp_header->source) != scan->port)
		return 0;
	if (scan->scan == SYN_FLAG)
	{
		if ((tcp_header->syn && tcp_header->ack) || (tcp_header->syn && !tcp_header->rst))
			scan->state = OPENED_STATE;
		else if (tcp_header->rst)
			scan->state = CLOSED_STATE;
	}
	if (scan->scan == NULL_FLAG || scan->scan == FIN_FLAG || scan->scan == XMAS_FLAG)
	{
		if (tcp_header->rst)
			scan->state = CLOSED_STATE;	
	}
	if (scan->scan == ACK_FLAG)
	{
		if (tcp_header->ack)
			scan->state = UNFILTERED_STATE;
	}
	return 1;
}

uint8_t handle_udp(struct udphdr *udp_header, t_scanres *scan)
{
	if (DEBUG_UDP)
		print_udpheader(udp_header);
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

uint8_t handle_icmp(struct icmphdr *icmp_header, t_scanres *scan)
{
	if (DEBUG_ICMP)
		print_icmpheader(icmp_header);
	if (scan->scan == SYN_FLAG)
	{								//3
		if ((icmp_header->type == ICMP_DEST_UNREACH) && acceptable_code(SYN_FLAG, icmp_header->code))
		{
			scan->state = FILTERED_STATE;
			return 1;
		}
	}
	if (scan->scan == NULL_FLAG || scan->scan == FIN_FLAG || scan->scan == XMAS_FLAG)
	{
		if ((icmp_header->type == ICMP_DEST_UNREACH) && acceptable_code(NULL_FLAG, icmp_header->code))
		{
			scan->state = FILTERED_STATE;
			return 1;
		}
	}
	if (scan->scan == ACK_FLAG)
	{
		if ((icmp_header->type == ICMP_DEST_UNREACH) && acceptable_code(ACK_FLAG, icmp_header->code))
		{
			scan->state = FILTERED_STATE;
			return 1;
		}
	}
	if (scan->scan == UDP_FLAG)
	{
		if ((icmp_header->type == ICMP_DEST_UNREACH) && acceptable_code(UDP_FLAG, icmp_header->code))
		{
			scan->state = FILTERED_STATE;
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
	if (protocall == 0)
		belong = handle_icmp((struct icmphdr *)(buffer + sizeof(struct ip)),
		scan);
	if (protocall == 1)
		belong = handle_udp((struct udphdr *)(buffer + sizeof(struct ip)),
		scan);
	if (protocall == 2)
		belong = handle_tcp((struct tcphdr *)(buffer + sizeof(struct ip)),
		scan);
	return belong;
}