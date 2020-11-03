#ifndef FT_NMAP_H
# define FT_NMAP_H

# include <libft.h>
# include <stdlib.h>
# include <stdint.h>
# include <stdio.h>
# include <netinet/ip_icmp.h>
# include <netinet/udp.h>
# include <netinet/tcp.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <arpa/inet.h>
# include <netdb.h>
# include <sys/time.h>
# include <pthread.h>
# include <sys/select.h>
# include <math.h>
# include <ifaddrs.h>

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

#define T_MAX			250
#define SCAN_MAX		6
#define MAX_PORTS		1024
#define MAX_HOST		(1024 / 2)

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

enum TcpSeq
{
	SYN_SEQ = 10,
	NULL_SEQ = 20,
	ACK_SEQ = 30,
	FIN_SEQ = 40,
	XMAS_SEQ = 50,
};

enum ScanPort
{
	SYN_PORT = 35450,
	NULL_PORT = 35460,
	ACK_PORT = 35470,
	FIN_PORT = 35480,
	XMAS_PORT = 35490,
	UDP_PORT = 35500,
};

enum StateType
{
	STATE_CLOSED = 1,
	STATE_OPENED = 2,
	STATE_FILTERED = 3,
	STATE_UNFILTERED = 4,
	STATE_OPEN_FILTERED = 5,
};

typedef struct		s_params
{
	uint32_t		count;
	uint32_t		payload_size;
	uint8_t			verbose;
	int32_t 		protocol;
	int32_t			af;
	uint8_t			parsed_payload_size;
	int				*ports;
	uint16_t		portcnt;
	uint8_t			thread_cnt;
	size_t			scan_timeout;
	uint16_t		scan_max_retry;
	uint16_t		host_port;
	uint8_t			parsed_scan;
}					t_params;

typedef struct		s_env
{
	t_params		params;

	//char			**hostname;			// host str from argv(Input)
	//struct in_addr	**dst_bin;			// host bin from getaddrinfo
	//char			*dst_name;			// host str from inet_ntop
	//char			*dst_subname;		// host str from gethostbyaddr
	//struct sockaddr	*dst_sockaddr;		// host sockaddr from getaddrinfo
	//uint16_t		dst_sockaddrlen;	// len of sockaddr from getaddrinfo
	//uint32_t		src_s_addr;			// src addr from getifaddrs
	struct hostent	**hosts;
	int 			hostcnt;
	uint8_t			running;
	struct s_worker	*threads;
	uint8_t			number_scans;
	uint8_t			scan_list[SCAN_MAX];
	uint16_t		ppt;
	size_t			start_time;
	char			*buff;
}					t_env;

typedef struct		s_scan_datas
{
	uint8_t			type;
	size_t			last_scan;
	uint16_t		retry;
	uint8_t			state;
	uint8_t			sent;
	uint8_t			finished;
}					t_scan_datas;

typedef struct		s_port_result
{
	uint16_t		port;
	uint8_t			finished;
	t_scan_datas	*scans;
}					t_port_result;

typedef struct		s_worker
{
	t_env			*env;
	int32_t			icmp_socket;
	int32_t			tcp_socket;
	int32_t			udp_socket;
	uint16_t		start_port;
	uint16_t		end_port;
	t_port_result	*ports_result;
	pthread_t		thread;
	uint8_t			running;
	fd_set			read_set_tcp;
	fd_set			read_set_udp;
	fd_set			read_set_icmp;
	struct timeval	select_timeout;
}					t_worker;

typedef struct		s_icmp_response_packet
{
	struct ip		ip_hdr;
	struct icmphdr	icmp_hdr;
	struct ip		ip_hdr_encaps;
	struct udphdr	udp_hdr;
}					t_icmp_response_packet;

typedef struct		s_tcp_packet
{
	struct ip		ip_hdr;
	struct tcphdr	tcp_hdr;
}					t_tcp_packet;

typedef struct		s_pseudo_hdr
{
	uint32_t		src;
	uint32_t		dst;
	uint8_t			reserved;
	uint8_t			prot;
	uint16_t		len;
}					t_pseudo_hdr;

typedef struct		s_udp_packet
{
	struct ip		ip_hdr;
	struct udphdr	udp_hdr;
}					t_udp_packet;

void				ft_error(char *msg, int errcode);
void				create_env(t_env *env);
void				display_help(int32_t exitcode);
void 				parse_args(int argc, char **argv, t_env *env);
u_int64_t			char_count(char *str, char c);
void				arg_err(char *argname);
int					in_arr(int val, uint8_t *arr);
void				binary_sort(int *a, int n);
int					remove_duplicates(int *arr, int n);

void				parse_ports(t_env *env, char *ports);
void				optimize_ports(t_params *params);

void				process_port(char **argv, int argc, int i, t_env *env);
void				process_speedup(char **argv, int argc, int i, t_env *env);
void				process_scan(char **argv, int argc, int i, t_env *env);
void				process_ip(char **argv, int argc, int i, t_env *env);
void				process_file(char **argv, int argc, int i, t_env *env);
int					get_host(t_env *env, const char *hostname);

void				optimize_hosts(t_env *env);
char				*ft_strtolower(char *str);
char				*ft_strtoupper(char *str);
void				set_defaults(t_env *env);

void				display_header(t_env *env);
//void 				print_invalid_param(uint8_t param, uint32_t index);
//void 				print_unknown_dst(char *dst);
//void 				resolve_host(t_env *env);
//void 				create_socket(t_env *env);
//void 				build_packet(t_env *env);
//void 				build_icmp_checksum(t_env *env);
//size_t				get_time();
//void				send_packet(t_env *env);
//void				receive_packet(t_env *env);
//void				create_threads(t_env *env);
//void				create_sockets(t_worker *worker);
//void				send_scan(t_worker *worker, t_port_result *port_result, t_scan_datas *scan_datas);
//void				build_tcp_packet(t_tcp_packet *packet, t_worker *worker, t_port_result *port_result);
//void				forge_tcp_header(struct tcphdr *tcp_hdr, t_worker *worker, t_port_result *port_result);
//void				build_udp_packet(t_udp_packet *packet, t_worker *worker, t_port_result *port_result);
//void				forge_udp_header(struct udphdr *udp_hdr, t_worker *worker, t_port_result *port_result);
//void				forge_ip_header(struct ip *ip_hdr, t_worker *worker);
//void				get_local_ip(t_env *env);
//uint16_t			build_tcp_checksum(t_tcp_packet *tcp_packet, t_worker *worker);
//uint16_t			build_udp_checksum(t_udp_packet *tcp_packet, t_worker *worker);
//void				print_configuration(t_env *env);
//void				print_result(t_env *env);

#endif
