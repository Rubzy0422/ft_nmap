/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_ip.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcoetzer <rcoetzer@student.wethinkcode.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/23 11:14:17 by rcoetzer          #+#    #+#             */
/*   Updated: 2020/11/03 18:32:25 by rcoetzer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_nmap.h>

void optimize_hosts(t_env *env)
{
	env->hosts = realloc(env->hosts, sizeof(struct hostent *) * env->hostcnt);
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

