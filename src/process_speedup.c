/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_speedup.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcoetzer <rcoetzer@student.wethinkcode.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/23 11:17:23 by rcoetzer          #+#    #+#             */
/*   Updated: 2020/11/19 10:59:06 by rcoetzer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_nmap.h>

void process_speedup(char **argv, int argc, int i, t_env *env)
{
	char *threads;
	int threadcount;

	if (i + 1 < argc)
		threads = argv[i + 1];
	else if (i + 1 >= argc)
		arg_err("--speedup");

	threadcount = ft_atoi(threads);
	if (!threadcount || threadcount <= 0 || threadcount > THREADMAX)
		ft_error("Error, invalid thread amount provided use 1-250", 0);
	env->params.thread_cnt = threadcount;
}