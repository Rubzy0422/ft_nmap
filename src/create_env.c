/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcoetzer <rcoetzer@student.wethinkcode.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/03 12:23:47 by rcoetzer          #+#    #+#             */
/*   Updated: 2020/11/03 18:24:42 by rcoetzer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_nmap.h>

void create_env(t_env *env)
{
	env->params.count = 3;
	env->params.payload_size = 0; // not bad
	env->params.verbose = 0;
	env->params.protocol = IPPROTO_ICMP;
	env->params.af = AF_INET;
	env->params.parsed_payload_size = 0;
	env->params.portcnt = 0;
	ft_memset(&env->params.ports, -1 ,sizeof(env->params.ports));
	//env->params.min_port = 1024;
	//env->params.max_port = 1024;
	env->params.thread_cnt = 1;
	env->params.scan_timeout = 300;
	env->params.scan_max_retry = 5;
	env->params.host_port = 54351;
	env->params.parsed_scan = 0;
	//env->hostname = NULL;
	//env->dst_bin = NULL;
	//env->dst_name = NULL;
	//env->dst_subname = NULL;
	//env->dst_sockaddr = NULL;
	//env->dst_sockaddrlen = 0;
	env->running = 1;
	env->number_scans = 0;
	env->hostcnt = 0;
	//ft_memset(env->scan_list, 0, sizeof(env->scan_list));
	//for (int scan= 0 ;scan < env->number_scans; scan++)
	//	env->scan_list[scan] = (1 << scan);
	env->params.ports = (int *)malloc(sizeof(int) * MAX_PORTS);
	env->hosts = (struct hostent **)malloc(sizeof(struct hostent *) * MAX_HOST);
}