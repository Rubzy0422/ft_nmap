/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcoetzer <rcoetzer@student.wethinkcode.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/19 11:36:27 by rcoetzer          #+#    #+#             */
/*   Updated: 2020/11/19 11:36:34 by rcoetzer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_nmap.h>

float time_sub(struct timeval *out, struct timeval *in)
{
	float sec = (float)(out->tv_sec - in->tv_sec) * 1000.0f;
	float usec = (float)(out->tv_usec - in->tv_usec) * 0.001f; // / 1000.0f
	return (float)(sec+ usec); 
}

int timeout(struct timeval st)
{
	struct timeval et;
	gettimeofday(&et, NULL);
	float sub = time_sub(&et,&st);

	if (sub >= MSTIMEOUT)
		return 1;
	else if (sub < 0 )
		return 1;
	return 0;
}
