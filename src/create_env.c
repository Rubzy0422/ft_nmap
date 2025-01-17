/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcoetzer <rcoetzer@student.wethinkcode.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/03 12:23:47 by rcoetzer          #+#    #+#             */
/*   Updated: 2020/11/19 11:03:06 by rcoetzer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_nmap.h>

void create_env(t_env *env)
{
	env->params.portcnt = 0;
	ft_memset(&env->params.ports, -1 ,sizeof(env->params.ports));
	env->params.thread_cnt = 0;
	env->params.scan_max_retry = 3;
	env->params.scancnt = 0;
	env->params.hostcnt = 0;
	env->params.ports = (int *)malloc(sizeof(int) * MAX_PORTS);
	env->params.hosts = (struct sockaddr_in *)malloc(sizeof(struct sockaddr_in)
						* MAX_HOST);
}