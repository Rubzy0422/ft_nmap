/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hostinfo.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcoetzer <rcoetzer@student.wethinkcode.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/17 12:24:12 by rcoetzer          #+#    #+#             */
/*   Updated: 2020/11/03 18:23:27 by rcoetzer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_nmap.h>

int		get_host(t_env *env, const char *hostname)
{
	env->hosts[env->hostcnt] = gethostbyname(hostname);
	if (!env->hosts[env->hostcnt])
	{
		herror(hostname);
		printf("Failed to resolve \"%s\".\n", hostname);
		free(hostname);
		return 0;
	}
	else
	{
		free(hostname);
		env->hostcnt++;
	}
	return 1;
}