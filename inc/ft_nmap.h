#ifndef FT_NMAP_H
# define FT_NMAP_H

#define _GNU_SOURCE
#define _OPEN_THREADS 

# include <stdio.h>
# include <arpa/inet.h>
# include <string.h>
# include <stdlib.h>
# include <netinet/in.h>
# include <net/ethernet.h>
# include <ctype.h>
# include <stdint.h>
# include <netinet/ip.h>
# include <netinet/ip_icmp.h>
# include <netinet/udp.h>
# include <netinet/tcp.h>
# include <sys/types.h>
# include <unistd.h>
# include <sys/socket.h>
# include <netdb.h>
# include <sys/time.h>
# include <pthread.h>
# include <ifaddrs.h>
# include <libft.h>
# include <signal.h>
# include <sys/select.h>
# include <poll.h>

#define DEBUG_ICMP			0
#define DEBUG_TCP			0
#define DEBUG_UDP			0
#define RETRIES				5
#define MSTIMEOUT			1000
#define ETHER_HEAD_LEN		14
#define THREADMAX			250
#define SCAN_MAX			6
#define MAX_PORTS			1024
#define MAX_HOST			(1024 / 2)

#define PACKET_SIZE			4096
#define IP_SIZE				13 //32
#define TTL					64

#define	SYN_FLAG			1 << 0
#define	NULL_FLAG			1 << 1
#define	ACK_FLAG			1 << 2
#define	FIN_FLAG			1 << 3
#define	XMAS_FLAG			1 << 4
#define	UDP_FLAG			1 << 5

#define	ACK_PORT			33255
#define	UDP_PORT			35500
#define	XMAS_PORT			40692
#define	FIN_PORT			48214
#define	NULL_PORT			58654
#define	SYN_PORT			65085

#define	CLOSED_STATE		1<< 0
#define	OPENED_STATE		1<< 1
#define	FILTERED_STATE		1<< 2
#define	UNFILTERED_STATE	1<< 3
#define	OF_STATE			1<< 4

typedef struct				s_scanres
{
	int						scancnt;
	uint8_t					state;
	struct sockaddr_in		host;
	uint32_t				port;
	uint8_t					scan;
	uint8_t					resp;
}							t_scanres;

typedef struct				s_threaddata {
	pthread_mutex_t			*lock;
	pthread_t				tid;
	t_scanres				scan;
	in_addr_t				source_ip;
}							t_threaddata;

typedef struct				s_params
{
	int						*ports;
	uint16_t				portcnt;
	struct sockaddr_in		*hosts;
	int 					hostcnt;
	uint8_t					thread_cnt;
	uint16_t				scan_max_retry;
	uint16_t				host_port;
	uint8_t					parsed_scan;
	uint8_t					scancnt;
	uint8_t					scan_list[SCAN_MAX];
}							t_params;

typedef struct				s_env
{
	t_params				params;
	uint8_t					running;
	size_t					start_time;
	char					*buff;
}							t_env;

struct						pseudo_header
{
	u_int32_t				source_address;
	u_int32_t				dest_address;
	u_int8_t				placeholder;
	u_int8_t				protocol;
	u_int16_t				pack_length;
};

typedef struct				s_tcpflags
{
	uint16_t				fin;
	uint16_t				syn;
	uint16_t				rst;
	uint16_t				psh;
	uint16_t				ack;
	uint16_t				urg;
}							t_tcpflags;

typedef struct				s_tcppack
{
	char					datagram[PACKET_SIZE];
	char					source_ip[IP_SIZE];
	char					*data;
	char					*pseudogram;
	struct iphdr			*iph;
	struct tcphdr			*tcph;
	struct pseudo_header	psh;
}							t_tcppack;

typedef struct				s_udppack
{
	char					datagram[PACKET_SIZE];
	char					source_ip[IP_SIZE];
	char					*data;
	char					*pseudogram;
	struct iphdr			*iph;
	struct udphdr			*udph;
	struct pseudo_header	psh;
}							t_udppack;

typedef struct				s_icmprespack
{
	struct ip				iph;
	struct icmphdr			icmph;
	struct ip				iphs;
	struct udphdr			udph;
}							t_icmprespack;

void						arg_err(char *argname);
int							binary_search(int *a, int item, int low, int high);
void						binary_sort(int *a, int n);
int 						remove_duplicates(int *arr, int n);
void 						create_env(t_env *env);
void 						set_defaults(t_env *env);
void 						display_scans(uint8_t scan_list[SCAN_MAX],
							uint8_t scancnt);
void 						display_header(t_env *env);
void						display_help(int32_t exitcode);
void						ft_error(char *msg, int errcode);
void						ft_free(void *mem);
int							get_host(t_env *env, char *hostname);
void						ft_lstappend(t_list *lst, t_list *new);
t_list						*ft_lstpop(t_list **lst);
void						parse_args(int argc, char **argv, t_env *env);
void						process_file(char **argv, int argc, int i,
							t_env *env);
void						optimize_hosts(t_env *env);
void						process_ip(char **argv, int argc,int i,t_env *env);
int							ft_strisdigit(char *str);
int							valid_port(char *port);
void						add_ports_range(t_env *env, char *split0,
							char *split1);
void						parse_port_part(t_env *env, char *part);
void						parse_ports(t_env *env, char *ports);
void						optimize_ports(t_params *params);
void						process_port(char **argv, int argc, int i,
							t_env *enft_callocv);
void 						process_scan(char **argv, int argc, int i,
							t_env *env);
void 						process_speedup(char **argv, int argc, int i,
							t_env *env);
uint64_t					char_count(char *str, char c);
char						*ft_strtoupper(char *str);
char						*ft_strtolower(char *str);
int							in_arr(int val, uint8_t *arr, int arrlen);
int							ft_strisdigit(char *str);

t_list						*init_scanlst(t_params *params);
in_addr_t					get_own_ip(void);
unsigned short				csum(unsigned short *ptr,int nbytes);
void						init_tcp(t_tcppack *tcp_pack,struct sockaddr_in sin, 
							in_addr_t source_ip,int src_port, int dest_port,
							t_tcpflags flags);
void						send_syn(struct sockaddr_in sin,in_addr_t source_ip, 
							int sock, int portnum);
void						send_null(struct sockaddr_in sin,in_addr_t source_ip, 
							int sock, int portnum);
void						send_ack(struct sockaddr_in sin,in_addr_t source_ip, 
							int sock, int portnum);
void						send_fin(struct sockaddr_in sin,in_addr_t source_ip, 
							int sock, int portnum);
void						send_xmas(struct sockaddr_in sin,in_addr_t source_ip, 
							int sock, int portnum);
void						init_udp(t_udppack *udp_pack,struct sockaddr_in sin,
							in_addr_t source_ip,int src_port, int dest_port);
void						send_udp(struct sockaddr_in sin,in_addr_t source_ip,
							int sock, int portnum);
uint8_t 					ProcessPacket(unsigned char* buffer,int protocall,
							in_addr_t *source_ip, t_scanres *scan);

int							acceptable_code(int flag, int code);
uint8_t						handle_udp(struct udphdr *udp_header,
							t_scanres *scan);
uint8_t						handle_tcp(struct tcphdr *tcp_header,
							t_scanres *scan);
uint8_t						handle_icmp(t_icmprespack *icmp, t_scanres *scan);
void						send_scan(t_scanres *scan, in_addr_t source_ip,
							int s);
float						time_sub(struct timeval *out, struct timeval *in);
void						*send_recv_pack(void *scan_mem);
void						run_scans(t_list *lst, t_env *env, 
							struct timeval tstart);
void						print_icmpheader(struct icmphdr * icmp_header);
void						print_tcpheader(struct tcphdr *tcph);
void						print_udpheader(struct udphdr *udph);
void						display_data(t_scanres *results, int scannum,
							t_params *params, struct timeval tstart);
int							timeout(struct timeval st);
void						*ft_realloc(void *ptr, size_t size);
void						*malzero(size_t size);
int							splitcnt(char **split);
#endif