/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_ports.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcoetzer <rcoetzer@student.wethinkcode.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/12 21:48:34 by rcoetzer          #+#    #+#             */
/*   Updated: 2020/11/19 12:58:33 by rcoetzer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_nmap.h>

void optimize_ports(t_params *params)
{
	binary_sort(params->ports, params->portcnt); 
	params->portcnt = remove_duplicates(params->ports, params->portcnt);
	params->ports = ft_realloc(params->ports, sizeof(int) * params->portcnt);
}

void parse_ports(t_env *env, char *ports)
{
	char **splitted;
	int i;

	if (!(splitted = ft_strsplit(ports, ',')))
		ft_error("ft_nmap: can't malloc splitted ports", 0);
	i = 0;
	while (splitted[i])
	{
		parse_port_part(env, splitted[i]);
		ft_free(splitted[i]);
		i++;
	}
	ft_free(splitted);
}