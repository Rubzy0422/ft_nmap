/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   defaults.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcoetzer <rcoetzer@student.wethinkcode.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/03 18:48:30 by rcoetzer          #+#    #+#             */
/*   Updated: 2020/11/05 22:29:14 by rcoetzer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_nmap.h>

void set_defaults(t_env *env)
{
	if (!env->params.hostcnt)
		ft_error("ft_nmap requires a host to scan", EXIT_FAILURE);
	if (!env->params.portcnt)
	{
		parse_ports(env, "1-1024");
		optimize_ports(&env->params);
	}
	optimize_hosts(env);
	if (!env->params.scancnt)
	{
		int i;
		for (i = 0; i < SCAN_MAX; i++)
			env->params.scan_list[env->params.scancnt++] = (1 << i);
	}
}