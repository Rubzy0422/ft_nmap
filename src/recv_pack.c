/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   recv_pack.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcoetzer <rcoetzer@student.wethinkcode.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/04 20:30:08 by rcoetzer          #+#    #+#             */
/*   Updated: 2020/11/11 23:04:45 by rcoetzer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_nmap.h>

void print_icmp_header(struct icmphdr * icmp_header)
{
	printf("CHECKSUM : %d\t\n", icmp_header->checksum);
	printf("CODE : \t\t%d\n", icmp_header->code);
	printf("TYPE : \t\t%d\n", icmp_header->type);
	printf("ECHO ID :\t%d\n", icmp_header->un.echo.id);
	printf("ECHO SEQ : \t%d\n", icmp_header->un.echo.sequence);
	printf("GATEWAY : \t%d\n", icmp_header->un.gateway);
	printf("\n\n");
}

//void print_tcpheader(struct tcphdr *tcph)
//{
//	printf("Source Port:\t\t%d\n", ntohs(tcph->source));
//	printf("Dest Port:\t\t%d\n", ntohs(tcph->dest));
//	printf("tcp SEQ:\t\t%d\n", tcph->seq);
//	printf("ACK_SEQ:\t\t%d\n", tcph->ack_seq);
//	printf("DOFF:\t\t\t%d\n", tcph->doff);
//	printf("FIN FLAG:\t\t%d\n", tcph->fin);	
//	printf("SYN FLAG:\t\t%d\n", tcph->syn);
//	printf("RST FLAG:\t\t%d\n", tcph->rst);
//	printf("PSH FLAG:\t\t%d\n", tcph->psh);
//	printf("ACK FLAG:\t\t%d\n", tcph->ack);
//	printf("URG FLAG:\t\t%d\n", tcph->urg);
//	printf("WINDOW:\t\t\t%d\n", ntohs(tcph->window));
//	printf("CHECK:\t\t\t%d\n", tcph->check);
//	printf("URG PTR:\t\t%d\n", tcph->urg_ptr);
//	printf("\n\n");
//	return ; 
//}

void handle_tcp(const u_char* packet,int ethernet_header_length,int ip_header_length, t_threaddata *tdata)
{
	t_scan cscan = tdata->scan[tdata->curr_scan]; // lol
	pthread_mutex_unlock(tdata->locktdata);
	struct tcphdr *tcp_header;

	tcp_header = (struct tcphdr *)(packet + ethernet_header_length + ip_header_length); 
	pthread_mutex_lock(tdata->locktdata);
	if (cscan.scan == SYN_FLAG)
	{
		if ((tcp_header->syn && tcp_header->ack) || (tcp_header->syn && !tcp_header->rst))
			tdata->result_data[tdata->offset + tdata->curr_scan].state = OPENED_STATE;
		else if (tcp_header->rst)
			tdata->result_data[tdata->offset + tdata->curr_scan].state = CLOSED_STATE;
	}
	if (cscan.scan == NULL_FLAG || cscan.scan == FIN_FLAG || cscan.scan == XMAS_FLAG)
	{
		if (tcp_header->rst)
			tdata->result_data[tdata->offset + tdata->curr_scan].state = CLOSED_STATE;	
	}
	if (cscan.scan == ACK_FLAG)
	{
		if (tcp_header->ack)
			tdata->result_data[tdata->offset + tdata->curr_scan].state = UNFILTERED_STATE;
	}
	pthread_mutex_unlock(tdata->locktdata);
	return;
}

void handle_udp()
{
	printf("UDP :)\n");
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

void handle_icmp(const u_char* packet,int ethernet_header_length,int ip_header_length, t_threaddata *tdata)
{
	t_scan cscan = tdata->scan[tdata->curr_scan]; // lol
	pthread_mutex_unlock(tdata->locktdata);
	struct icmphdr *icmp_header;

	icmp_header = (struct icmphdr *)(packet + ethernet_header_length + ip_header_length); 
	
	pthread_mutex_lock(tdata->locktdata);
	//print_icmp_header(icmp_header);
	if (cscan.scan == SYN_FLAG)
	{								//3
		if ((icmp_header->type == ICMP_DEST_UNREACH) && acceptable_code(SYN_FLAG, icmp_header->code))
			tdata->result_data[tdata->offset + tdata->curr_scan].state = FILTERED_STATE;
	}
	if (cscan.scan == NULL_FLAG || cscan.scan == FIN_FLAG || cscan.scan == XMAS_FLAG)
	{
		if ((icmp_header->type == ICMP_DEST_UNREACH) && acceptable_code(NULL_FLAG, icmp_header->code))
			tdata->result_data[tdata->offset + tdata->curr_scan].state = FILTERED_STATE;
	}
	if (cscan.scan == ACK_FLAG)
	{
		if ((icmp_header->type == ICMP_DEST_UNREACH) && acceptable_code(ACK_FLAG, icmp_header->code))
			tdata->result_data[tdata->offset + tdata->curr_scan].state = FILTERED_STATE;
	}
	if (cscan.scan == UDP_FLAG)
	{
		if ((icmp_header->type == ICMP_DEST_UNREACH) && acceptable_code(UDP_FLAG, icmp_header->code))
			tdata->result_data[tdata->offset + tdata->curr_scan].state = FILTERED_STATE;
	}
	pthread_mutex_unlock(tdata->locktdata);
	return;
}

void my_packet_handler(u_char *args, const struct pcap_pkthdr* header, const u_char* packet)
{
	printf("START PACKET HANDLER!");
	t_threaddata *tdata = (t_threaddata *)args;
	pthread_mutex_unlock(tdata->locktdata);
	header = (void *)header;
	struct ether_header *eth_header;
	eth_header = (struct ether_header *)packet;
	if (ntohs(eth_header->ether_type) == ETHERTYPE_IP)
	{
		const u_char *ip_header;
		int ethernet_header_length = ETHER_HEAD_LEN; 
		int ip_header_length;
		ip_header = packet + ethernet_header_length;
		ip_header_length = ((*ip_header) & 0x0F);
		ip_header_length = ip_header_length * 4;
		u_char protocol = *(ip_header + 9);
	
		if (protocol == IPPROTO_TCP)
		{
			pthread_mutex_lock(tdata->locktdata);
			handle_tcp(packet, ethernet_header_length, ip_header_length, tdata);
		}
		else if (protocol == IPPROTO_UDP)
		{
			handle_udp();
		}
		else if (protocol == IPPROTO_ICMP)
		{
			pthread_mutex_lock(tdata->locktdata);
			handle_icmp(packet, ethernet_header_length, ip_header_length, tdata);
		}
	}
	return;
}

void send_scan(t_scan *scan, in_addr_t source_ip, int s) {
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



void *recv_scan_res(bpf_u_int32 ip_raw, t_scan scan, pcap_t *handle, in_addr_t source_ip, int s, t_threaddata *tdata)
{
	struct bpf_program filter;
	char *filter_exp;
	char s_ip[INET_ADDRSTRLEN];
	struct pcap_pkthdr *packet_header;

	if (!inet_ntop(AF_INET, &source_ip, s_ip, sizeof(s_ip)))
		ft_error("inet_ntop", EXIT_FAILURE);
	asprintf(&filter_exp, "ip and (udp or tcp or icmp) and port %d and dst net %s", scan.port, s_ip);
	
	//Timeout using signal :) 
	// how does one timeout? 
	if (pcap_compile(handle, &filter, filter_exp, 0, ip_raw) == -1) {
		printf("Bad filter - %s\n", pcap_geterr(handle));
		return (void *)2;
	}
	if (pcap_setfilter(handle, &filter) == -1) {
		printf("Error setting filter - %s\n", pcap_geterr(handle));
		return (void *)2;
	}
	send_scan(&scan, source_ip, s);
	//Need to set responce based on current scan :)
	pthread_mutex_lock(tdata->locktdata);
	//alarm(1);
	//pcap_next ? 
	//packet_header = (struct pcap_pkthdr *)malloc(sizeof(struct pcap_pkthdr));

	//const u_char *packet = pcap_next(handle, packet_header);
	//if (packet == NULL) {
	//	printf("No packet found.\n");
	//}
	//my_packet_handler((u_char *)tdata, packet_header, packet);
	
	printf("WHY?\n");
	//free(packet_header);
	pcap_dispatch(handle, 0, my_packet_handler, (u_char *)tdata);
	//printf("RES : %d\n", res);
	return (void *)0;
}

void set_recv_defaults(t_scan *scan, int scancount, int offset, t_scanres *res)
{
	int i = 0;
	while (i < scancount)
	{
		res[offset + i].host = scan[i].host;
		res[offset + i].port = scan[i].port;
		res[offset + i].scan = scan[i].scan;
		if (scan[offset +i].scan == SYN_FLAG)
			res[offset + i].state = FILTERED_STATE;
		if (scan[offset +i].scan == UDP_FLAG)
			res[offset + i].state = OPEN_FILTERED_STATE;
		if (scan[offset +i].scan == NULL_FLAG || scan[offset +i].scan == FIN_FLAG 
		|| scan[offset +i].scan == XMAS_FLAG)
			res[offset + i].state = OPEN_FILTERED_STATE;
		if (scan[offset +i].scan == ACK_FLAG)
			res[offset + i].state = FILTERED_STATE;
		i++;
	}
}

void *recv_pack(void *data) {
	pcap_if_t *interfaces;
	bpf_u_int32 ip_raw;
	bpf_u_int32 subnet_mask_raw;
	pcap_t *handle;
	interfaces = NULL;
	char error_buffer[PCAP_ERRBUF_SIZE]; 
	int one = 1;
	int scancnt;
	int s;
	in_addr_t source_ip;
	t_scan scan;
	int currscan = 0;
	 
	t_threaddata *tdata = data;
	

	set_recv_defaults(tdata->scan, tdata->scancnt, tdata->offset, tdata->result_data);
	scancnt = tdata->scancnt;
	pthread_mutex_unlock(tdata->locktdata);
	source_ip = get_own_ip();
	s = socket (PF_INET, SOCK_RAW, IPPROTO_RAW);
	if(s == -1)
		ft_error("Failed to create socket", s);
	if (setsockopt (s, IPPROTO_IP, IP_HDRINCL, &one, sizeof (one)) < 0)
		ft_error("Error setting IP_HDRINCL", EXIT_FAILURE);
	//Setup em recv
	if(pcap_findalldevs(&interfaces,error_buffer)==-1)
	{
		fprintf(stderr, "\nerror in pcap findall devs :%s", error_buffer);
		return (void *)-1;
	}
	if (interfaces == NULL) {
		printf("Error finding device: %s\n", error_buffer);
		return (void *)1;
	}
	if (pcap_lookupnet(interfaces->name, &ip_raw, &subnet_mask_raw, error_buffer) == -1) {
		printf("Could not get information for device: %s\n", interfaces->name);
		ip_raw = 0;
		subnet_mask_raw = 0;
	}
	handle = pcap_open_live(interfaces->name, BUFSIZ, 0, 300, error_buffer);
	if (handle == NULL) {
		fprintf(stderr, "Could not open device %s: %s\n", interfaces->name, error_buffer);
		return (void *)2;
	}
	while (currscan < scancnt)
	{
		pthread_mutex_lock(tdata->locktdata);
		tdata->curr_scan = currscan;
		scancnt = tdata->scancnt;
		ft_memcpy(&scan, &tdata->scan[currscan], sizeof(t_scan));
		pthread_mutex_unlock(tdata->locktdata);
		recv_scan_res(ip_raw, scan, handle, source_ip, s, tdata); // timeout on this one plz :) 
		currscan++;
	}
	pcap_close(handle);
	pcap_freealldevs(interfaces);
	
	//pthread_mutex_lock(tdata->locktdata); 
	//ft_free(tdata->scan);
	//ft_free(tdata);
	//pthread_mutex_unlock(tdata->locktdata); 
	return (void *)0;
}