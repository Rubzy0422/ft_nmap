#ifndef FT_NMAP_H
# define FT_NMAP_H

#define _GNU_SOURCE         /* See feature_test_macros(7) */
# include <stdio.h>
# include <pcap.h>
# include <arpa/inet.h>
# include <string.h>
# include <stdlib.h>
# include <netinet/in.h>
# include <net/ethernet.h>
# include <ctype.h>
# include <stdint.h>
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
#include <signal.h>

#define ETHER_HEAD_LEN 14


#define RESET			"\033[0m"
#define BLACK			"\033[30m"
#define RED				"\033[31m"
#define GREEN			"\033[32m"
#define YELLOW			"\033[33m"
#define BLUE			"\033[34m"
#define MAGENTA			"\033[35m"
#define CYAN			"\033[36m"
#define WHITE			"\033[37m"
#define BOLDBLACK		"\033[1m\033[30m"
#define BOLDRED			"\033[1m\033[31m"
#define BOLDGREEN		"\033[1m\033[32m"
#define BOLDYELLOW		"\033[1m\033[33m"
#define BOLDBLUE		"\033[1m\033[34m"
#define BOLDMAGENTA		"\033[1m\033[35m"
#define BOLDCYAN		"\033[1m\033[36m"
#define BOLDWHITE		"\033[1m\033[37m"

#define SCAN_RETRY		3
#define T_MAX			250
#define SCAN_MAX		6
#define MAX_PORTS		1024
#define MAX_HOST		(1024 / 2)

#define PACKET_SIZE		4096
#define IP_SIZE			13 
//32
#define TTL				64

struct s_packet_node;
struct s_worker;

enum ScanType
{
	SYN_FLAG = 1 << 0,
	NULL_FLAG = 1 << 1,
	ACK_FLAG = 1 << 2,
	FIN_FLAG = 1 << 3,
	XMAS_FLAG = 1 << 4,
	UDP_FLAG = 1 << 5,
	MAX_FLAG = 1 << 6,
};

enum tcpSeq
{
	SYN_SEQ = 10,
	NULL_SEQ = 20,
	ACK_SEQ = 30,
	FIN_SEQ = 40,
	XMAS_SEQ = 50,
};

enum ScanPort
{
	ACK_PORT = 33255,
	UDP_PORT = 35500,
	XMAS_PORT = 40692,
	FIN_PORT = 48214,
	NULL_PORT = 58654,
	SYN_PORT = 65085,
};

enum StateType
{
	CLOSED_STATE = 1<< 0,
	OPENED_STATE = 1<< 1,
	FILTERED_STATE = 1<< 2,
	UNFILTERED_STATE = 1<< 3,
	OPEN_FILTERED_STATE = 1<< 4,
};

//User Input basically
typedef struct		s_params
{
	uint32_t		count;
	//uint32_t		payload_size;
	//uint8_t			verbose;
	//int32_t 		protocol;
	int32_t			af;
	//uint8_t			parsed_payload_size;
	int				*ports;
	uint16_t		portcnt;
	struct sockaddr_in *hosts;
	int 			hostcnt;
	uint8_t			thread_cnt;
	size_t			scan_timeout;
	uint16_t		scan_max_retry;
	uint16_t		host_port;
	uint8_t			parsed_scan;
	uint8_t			scancnt;
	uint8_t			scan_list[SCAN_MAX];
}					t_params;

typedef struct		s_env
{
	t_params		params;
	uint8_t			running;
	struct s_worker	*threads;
	uint16_t		ppt;
	size_t			start_time;
	char			*buff;
}					t_env;

struct				pseudo_header
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

typedef struct			s_scanres
{
	uint16_t			trycnt;
	uint8_t				state;
	struct sockaddr_in	host;
	uint32_t			port;
	uint8_t				scan;
}						t_scanres;



typedef struct	s_spt
{
	int			thread_ps;
	int			extra_ps;
}				t_spt;

typedef struct s_scan
{
	struct sockaddr_in	host;
	int				port;
	int				scan;
}				t_scan;

typedef struct	s_threaddata
{
	pthread_mutex_t *locktdata;
	pthread_t		thread;
	t_scan 			*scan;
	int				scancnt;
	int				curr_scan;
	int				pid;
	t_scanres		*result_data; //memaddress of results pass here :) 
	int				offset;
}				t_threaddata;


typedef struct s_pass
{
	t_scanres	**result_data; //memloc to put it in (pthread lock and unlock this plz) 
	t_scan		scan;
}				t_pass;

//typedef struct		s_worker
//{
//	t_env			*env;
//	int32_t			icmp_socket;
//	int32_t			tcp_socket;
//	int32_t			udp_socket;
//	int				*ports;
//	uint16_t		portcnt;
//	t_port_result	*port_res;
//	pthread_t		thread;
//	uint8_t			running;
//	fd_set			read_set_tcp;
//	fd_set			read_set_udp;
//	fd_set			read_set_icmp;
//	struct timeval	select_timeout;
//}					t_worker;

void				ft_free(void *mem);

void				ft_error(char *msg, int errcode);
void				create_env(t_env *env);
void				display_help(int32_t exitcode);
void 				parse_args(int argc, char **argv, t_env *env);
u_int64_t			char_count(char *str, char c);
void				arg_err(char *argname);
int					in_arr(int val, uint8_t *arr, int arrlen);
void				binary_sort(int *a, int n);
int					remove_duplicates(int *arr, int n);

void				parse_ports(t_env *env, char *ports);
void				optimize_ports(t_params *params);

void				process_port(char **argv, int argc, int i, t_env *env);
void				process_speedup(char **argv, int argc, int i, t_env *env);
void				process_scan(char **argv, int argc, int i, t_env *env);
void				process_ip(char **argv, int argc, int i, t_env *env);
void				process_file(char **argv, int argc, int i, t_env *env);
int					get_host(t_env *env, char *hostname);

void				optimize_hosts(t_env *env);
char				*ft_strtolower(char *str);
char				*ft_strtoupper(char *str);
void				set_defaults(t_env *env);

void				display_header(t_env *env);

//CREATE THE THREADS AND THEIR SOCKETS :) 

//Init tcp :) 
in_addr_t				get_own_ip(void);

unsigned short			csum(unsigned short *ptr,int nbytes);
void					init_tcp(t_tcppack *tcp_pack, struct sockaddr_in sin, 
						in_addr_t source_ip,int src_port, int dest_port,
						t_tcpflags flags);

void					send_syn(struct sockaddr_in sin, in_addr_t source_ip, 
						int sock, int portnum);
void					send_null(struct sockaddr_in sin, in_addr_t source_ip, 
						int sock, int portnum);
void					send_ack(struct sockaddr_in sin, in_addr_t source_ip, 
						int sock, int portnum);
void					send_fin(struct sockaddr_in sin, in_addr_t source_ip, 
						int sock, int portnum);
void					send_xmas(struct sockaddr_in sin, in_addr_t source_ip, 
						int sock, int portnum);

void					init_udp(t_udppack *udp_pack, struct sockaddr_in sin,
						in_addr_t source_ip,int src_port, int dest_port);
void					send_udp(struct sockaddr_in sin, in_addr_t source_ip,
						int sock, int portnum);

void					*recv_pack(void *tdata);
//void				create_threads(t_env *env);
//void				create_sockets(t_worker *worker);


#endif
