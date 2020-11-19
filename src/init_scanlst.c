/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_scanlst.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcoetzer <rcoetzer@student.wethinkcode.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/12 22:15:07 by rcoetzer          #+#    #+#             */
/*   Updated: 2020/11/19 11:18:34 by rcoetzer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_nmap.h>

int		get_timeout_state(uint8_t scantype)
{
	if (scantype == SYN_FLAG)
		return FILTERED_STATE;
	if (scantype == UDP_FLAG)
		return OF_STATE;
	if (scantype == XMAS_FLAG || scantype == NULL_FLAG || scantype == FIN_FLAG)
		return OF_STATE;
	if (scantype == ACK_FLAG)
		return FILTERED_STATE;
	ft_error("UNKNOWN SCAN ADDED!", EXIT_FAILURE);
	return -1;
}

t_list *create_item(t_params *params, int host, int port, int scan, int num)
{
	t_list *item;
	t_scanres scanres;

	ft_memcpy(&scanres.host, &params->hosts[host], sizeof(struct sockaddr_in));
	scanres.port = params->ports[port];
	scanres.scan = params->scan_list[scan];
	scanres.state = get_timeout_state(params->scan_list[scan]);
	scanres.scancnt = num;
	item = ft_lstnew(&scanres, sizeof(t_scanres));
	return item;
}

t_list *init_scanlst(t_params *params)
{
	t_list *lst;
	int host;
	int port;
	int scan;
	int num;

	host = 0;
	num = 0;
	lst = ft_lstnew(NULL, 0);
	while (host < params->hostcnt)
	{
		port = 0;
		while (port < params->portcnt)
		{
			scan = 0;
			while (scan < params->scancnt)
			{
				ft_lstappend(lst , create_item(params, host, port, scan, num));
				scan++;
				num++;
			}
			port++;
		}
		host++;
	}
	ft_free(params->hosts);
	ft_free(params->ports);
	return lst;
}