/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_own_ip.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcoetzer <rcoetzer@student.wethinkcode.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/04 15:35:29 by rcoetzer          #+#    #+#             */
/*   Updated: 2020/11/19 11:03:47 by rcoetzer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_nmap.h>

in_addr_t	get_own_ip()
{
	in_addr_t tmp;
	struct ifaddrs *origin;
	struct ifaddrs *lst;
	char ipset = 0;

	getifaddrs(&origin);
	lst = origin;
	while (lst)
	{
		if (!lst->ifa_addr)
		{
			lst = lst->ifa_next;
			continue;
		}
		if (lst->ifa_addr->sa_family != AF_INET)
		{
			lst = lst->ifa_next;
			continue;
		}
		if (!strcmp(lst->ifa_name, "lo"))
		{
			ipset |= 1;
			tmp = ((struct sockaddr_in*)lst->ifa_addr)->sin_addr.s_addr;
		}
		else if (ipset & 2)
		{
			fprintf(stderr, "ft_nmap: ip network collision!\n");
		}
		else
		{
			ipset |= 2;
			tmp = ((struct sockaddr_in*)lst->ifa_addr)->sin_addr.s_addr;
		}
		lst = lst->ifa_next;
	}
	if (origin)
	{
		freeifaddrs(origin);
		return tmp;
	}
	if (ipset == 3)
		return 0;
	ft_error("ft_nmap: can't resolve external ip", EXIT_FAILURE);
	return -1;
}