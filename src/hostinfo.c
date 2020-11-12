/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hostinfo.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcoetzer <rcoetzer@student.wethinkcode.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/17 12:24:12 by rcoetzer          #+#    #+#             */
/*   Updated: 2020/11/07 15:10:04 by rcoetzer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_nmap.h>

int		get_host(t_env *env, char *hostname)
{
	struct hostent *host = gethostbyname(hostname);
	if (!host)
	{
		herror(hostname);
		printf("Failed to resolve \"%s\".\n", hostname);
		ft_free(hostname);
		return 0;
	}
	else
	{
		ft_memcpy(&env->params.hosts[env->params.hostcnt].sin_addr, host->h_addr_list[0], host->h_length);
		ft_free(hostname);
		env->params.hostcnt++;
	}
	return 1;
}