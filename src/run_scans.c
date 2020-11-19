/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_scans.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcoetzer <rcoetzer@student.wethinkcode.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/19 11:25:50 by rcoetzer          #+#    #+#             */
/*   Updated: 2020/11/19 12:07:28 by rcoetzer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_nmap.h>

t_threaddata	*create_threads(t_params *params, pthread_mutex_t *lock)
{
	t_threaddata *data;

	data = malloc(sizeof(t_threaddata) * params->thread_cnt);
	if (!data)
		ft_error("Could not malloc for threads!", EXIT_FAILURE);
	for (int i =0; i< params->thread_cnt; i++)
	{
		data[i].lock = lock;
		data[i].source_ip = get_own_ip();
	}
	return data;
}

void cleanup_threads(t_threaddata *data, pthread_mutex_t *lock)
{
	ft_free(data);
	pthread_mutex_destroy(lock);
}

void recv_thread_resp(t_threaddata *tdata, int tiu, t_scanres *results)
{
	int i;
	int p_joinres;
	void *val;
	t_scanres *scan;

	scan = NULL;
	i = 0;
	while (i < tiu)
	{	
		p_joinres = pthread_join(tdata[i].tid, &val);
		if (p_joinres < 0)
			ft_error("could not join thread!\n", EXIT_FAILURE);
		scan = (t_scanres *)val;
		if (scan != NULL)
		{
			ft_memcpy(&results[scan->scancnt] ,scan, sizeof(t_scanres));
			ft_free(scan);
		}
		i++;
	}
}

void dispatch_scans(t_list *lst, uint8_t thread_cnt, t_threaddata *tdata, 
			t_scanres *results)
{
	int tiu;
	t_list *tmp;
	t_scanres res;
	
	tiu = 0;
	while (lst)
	{
		while (tiu < thread_cnt && lst)
		{
			tmp = ft_lstpop(&lst); 
			res = *(t_scanres *)tmp->content;
			ft_memcpy(&tdata[tiu].scan, &res, sizeof(t_scanres));
			pthread_mutex_lock(tdata[tiu].lock); //&lock
			pthread_create(&tdata[tiu].tid, NULL, send_recv_pack, &tdata[tiu]);
			tiu++;
			if (tmp)
				ft_lstdelone(&tmp, ft_del);
		}
		recv_thread_resp(tdata, tiu, results);
		tiu = 0; //reset thread
	}
}

void run_scans(t_list *lst, t_env *env, struct timeval tstart)
{
	pthread_mutex_t lock;
	t_threaddata *tdata;
	t_list *tmp;
	t_scanres *results;
	char s_ip[INET_ADDRSTRLEN];
	unsigned int scancnt;
	
	scancnt = env->params.hostcnt *env->params.portcnt * env->params.scancnt;
	if (pthread_mutex_init(&lock, NULL) != 0)
		ft_error("Mutex init failed", EXIT_FAILURE);
	tdata = create_threads(&env->params, &lock);	
	//Pop Null
	tmp = ft_lstpop(&lst);
	ft_lstdelone(&tmp, ft_del);
	if (!inet_ntop(AF_INET, &tdata->source_ip, s_ip, sizeof(s_ip)))
		ft_error("inet_ntop", EXIT_FAILURE);
	results = malloc(sizeof(t_scanres) * (scancnt));// +1?
	if (!results)
		ft_error("Could not malloc for the results!", EXIT_FAILURE);
	dispatch_scans(lst, env->params.thread_cnt, tdata, results);
	display_data(results, scancnt, &env->params, tstart);
	cleanup_threads(tdata, &lock);
}