/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_ip.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcoetzer <rcoetzer@student.wethinkcode.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/23 11:14:17 by rcoetzer          #+#    #+#             */
/*   Updated: 2020/11/19 12:58:38 by rcoetzer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_nmap.h>

void optimize_hosts(t_env *env)
{
	env->params.hosts = ft_realloc(env->params.hosts, sizeof(struct sockaddr_in) *
	env->params.hostcnt);
}

void process_ip(char **argv, int argc, int i, t_env *env)
{
	char *ip;
	
	if (i + 1 < argc)
		ip = argv[i + 1];
	else if (i + 1 >= argc)
		arg_err("--ip");
	if (!get_host(env, ft_strdup(ip)))
		exit(EXIT_FAILURE);
}

