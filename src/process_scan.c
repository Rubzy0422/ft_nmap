/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_scan.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcoetzer <rcoetzer@student.wethinkcode.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/23 11:22:58 by rcoetzer          #+#    #+#             */
/*   Updated: 2020/11/03 17:47:38 by rcoetzer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_nmap.h>

void process_scan(char **argv, int argc, int i, t_env *env)
{
	int		x;

	x = 1;
	while (i + x < argc)
	{
		if (!ft_strcmp("SYN", ft_strtoupper(argv[i + x])) && !in_arr(SYN_FLAG, env->scan_list))
			env->scan_list[env->number_scans++] = SYN_FLAG;
		else if (!ft_strcmp("NULL", ft_strtoupper(argv[i + x]))  && !in_arr(NULL_FLAG, env->scan_list))
			env->scan_list[env->number_scans++] = NULL_FLAG;
		else if (!ft_strcmp("ACK", ft_strtoupper(argv[i + x])) && !in_arr(ACK_FLAG, env->scan_list))
			env->scan_list[env->number_scans++] = ACK_FLAG;
		else if (!ft_strcmp("FIN", ft_strtoupper(argv[i + x])) && !in_arr(FIN_FLAG, env->scan_list))
			env->scan_list[env->number_scans++] = FIN_FLAG;
		else if (!ft_strcmp("XMAS", ft_strtoupper(argv[i + x])) && !in_arr(XMAS_FLAG, env->scan_list))
			env->scan_list[env->number_scans++] = XMAS_FLAG;
		else if (!ft_strcmp("UDP", ft_strtoupper(argv[i + x])) && !in_arr(UDP_FLAG, env->scan_list))
			env->scan_list[env->number_scans++] = UDP_FLAG;
		else
		{
			ft_strtolower(argv[i + x]);
			if (!env->number_scans)
				arg_err("--scan");
			break;
		}
		x++;
	}
	if (i + 1 >= argc)
		arg_err("--scan");
}