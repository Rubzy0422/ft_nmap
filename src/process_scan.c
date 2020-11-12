/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_scan.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcoetzer <rcoetzer@student.wethinkcode.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/23 11:22:58 by rcoetzer          #+#    #+#             */
/*   Updated: 2020/11/07 14:56:25 by rcoetzer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_nmap.h>

void process_scan(char **argv, int argc, int i, t_env *env)
{
	int		x;
	int		arrlen;

	ft_memset(env->params.scan_list, -1 ,sizeof(env->params.scan_list));
	x = 1;
	arrlen = 0;
	while (i + x < argc)
	{
		if (!ft_strcmp("SYN", ft_strtoupper(argv[i + x])) && !in_arr(SYN_FLAG, env->params.scan_list, arrlen))
			env->params.scan_list[env->params.scancnt++] = SYN_FLAG;
		else if (!ft_strcmp("NULL", ft_strtoupper(argv[i + x]))  && !in_arr(NULL_FLAG, env->params.scan_list, arrlen))
			env->params.scan_list[env->params.scancnt++] = NULL_FLAG;
		else if (!ft_strcmp("ACK", ft_strtoupper(argv[i + x])) && !in_arr(ACK_FLAG, env->params.scan_list, arrlen))
			env->params.scan_list[env->params.scancnt++] = ACK_FLAG;
		else if (!ft_strcmp("FIN", ft_strtoupper(argv[i + x])) && !in_arr(FIN_FLAG, env->params.scan_list, arrlen))
			env->params.scan_list[env->params.scancnt++] = FIN_FLAG;
		else if (!ft_strcmp("XMAS", ft_strtoupper(argv[i + x])) && !in_arr(XMAS_FLAG, env->params.scan_list, arrlen))
			env->params.scan_list[env->params.scancnt++] = XMAS_FLAG;
		else if (!ft_strcmp("UDP", ft_strtoupper(argv[i + x])) && !in_arr(UDP_FLAG, env->params.scan_list, arrlen))
			env->params.scan_list[env->params.scancnt++] = UDP_FLAG;
		else
		{
			ft_strtolower(argv[i + x]);
			if (!env->params.scancnt)
				arg_err("--scan");
			break;
		}
		x++;
	}
	if (i + 1 >= argc)
		arg_err("--scan");
}