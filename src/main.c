/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcoetzer <rcoetzer@student.wethinkcode.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/03 12:20:50 by rcoetzer          #+#    #+#             */
/*   Updated: 2020/11/11 19:30:29 by rcoetzer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_nmap.h>

t_spt get_spt(int scancnt, int threadcnt)
{
	t_spt spt;

	spt.thread_ps = scancnt / threadcnt;
	spt.extra_ps = 0;
	if (spt.thread_ps * threadcnt == scancnt)
		return spt;
	else 
		spt.extra_ps = scancnt - (spt.thread_ps * threadcnt);
	return spt;
}

void allocscan(t_threaddata *threads, t_spt spt, int tnum)
{
	//printf("Allocated thread %d\n", tnum);
	threads[tnum].scancnt = (tnum > spt.extra_ps - 1) ? spt.thread_ps : spt.thread_ps + 1;
	threads[tnum].scan = malloc(sizeof(t_scan) * threads->scancnt);
	if (!threads[tnum].scan)
		ft_error("could not malloc for thread scans!", EXIT_FAILURE);
}

void	get_thread_data(t_params *params, t_threaddata *threads, t_spt spt)
{
	int host = 0;
	int scan = 0;
	int port = 0;
	int cnt = 0;
	int cnt2 = 0;
	int tnum = 0;

	allocscan(threads, spt, tnum);
	while (host < params->hostcnt)
	{
		port = 0;
		while (port < params->portcnt)
		{
			scan = 0;
			while (scan < params->scancnt)
			{
				ft_memcpy(&threads[tnum].scan[cnt].host, &params->hosts[host], sizeof(params->hosts[host]));
				ft_memcpy(&threads[tnum].scan[cnt].port, &params->ports[port], sizeof(params->ports[port]));
				ft_memcpy(&threads[tnum].scan[cnt].scan, &params->scan_list[scan], sizeof(params->scan_list[scan]));
				cnt++;
				if ((cnt == spt.thread_ps && tnum >= spt.extra_ps) ||
				(cnt == spt.thread_ps + 1 && tnum < spt.extra_ps))
				{
					tnum++;
					if (tnum < params->thread_cnt)
					{
						allocscan(threads, spt, tnum);
						threads[tnum].offset = cnt2;
					}	
					cnt = 0;
				}
				scan++;
				cnt2++;
			}
			port++;
		}
		host++;
	}
	ft_free(params->hosts);
	ft_free(params->ports);
	params->thread_cnt = tnum;
}

int calculate_offset(int thread, t_spt spt)
{
	int offset;
	int i;

	i = 0;
	if (thread == 0)
		return 0;
	while (i < thread)
	{
		offset += spt.thread_ps;
		if (i < spt.extra_ps)
			offset++;
		i++;
	}
	return offset;
}

t_threaddata *launch_threads(t_params *params, int scancnt, t_scanres *result_data)
{
	t_threaddata *threads;
	t_spt spt;
	pthread_mutex_t locktdata;
	int thread;

	if (pthread_mutex_init(&locktdata, NULL) != 0)
		ft_error("mutex init has failed lock tdata", EXIT_FAILURE);
	if (params->thread_cnt == 0)
	{
		threads = malloc(sizeof(t_threaddata)); // malloc for main thread 
		if (!threads)
			ft_error("Could not malloc for threads!", EXIT_FAILURE);
		spt.thread_ps = params->hostcnt * params->portcnt * params->scancnt;
		spt.extra_ps = 0;
		threads[0].locktdata = &locktdata;
		threads[0].result_data = result_data;
		threads[0].offset = 0; // where to start recv buff at :) 
		get_thread_data(params, threads, spt);
		recv_pack(threads);
	}
	else {
		threads = malloc(sizeof(t_threaddata) * params->thread_cnt); 
		if (!threads)
			ft_error("Could not malloc for threads!", EXIT_FAILURE);
		spt = get_spt(scancnt, params->thread_cnt);
		get_thread_data(params, threads, spt);
		
		for (thread = 0; thread < params->thread_cnt; thread++)
		{
			threads[thread].result_data = result_data;
			threads[thread].locktdata = &locktdata;
			threads[thread].offset = calculate_offset(thread, spt);
				
			pthread_mutex_lock(threads[thread].locktdata);
			pthread_create(&threads[thread].thread , NULL, recv_pack, &threads[thread]);
		}
		for (thread = 0; thread < params->thread_cnt; thread++) 
			pthread_join(threads[thread].thread, NULL);
	}
	return threads;
}

void display_results(t_scanres *results)
{
	ft_free(results);
}

int main(int argc, char **argv)
{
	t_env env;
	struct timeval tstart;
	t_scanres *results;
	unsigned int scancnt;

	gettimeofday(&tstart, NULL);
	if (argc < 2)
		display_help(EXIT_FAILURE);
	if (getuid())
		ft_error("ft_nmap requires root privileges!", EXIT_FAILURE);
	create_env(&env);
	parse_args(argc, argv, &env); 
	set_defaults(&env);
	display_header(&env);
	scancnt = env.params.hostcnt *env.params.portcnt * env.params.scancnt;
	results = malloc(sizeof(t_scanres) * scancnt);
	if (!results)
		ft_error("Could not malloc for the results!", EXIT_FAILURE);
	ft_free(launch_threads(&env.params, scancnt, results));
	display_results(results);
	return EXIT_SUCCESS;
}