/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcoetzer <rcoetzer@student.wethinkcode.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/03 12:20:50 by rcoetzer          #+#    #+#             */
/*   Updated: 2020/11/03 19:08:31 by rcoetzer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_nmap.h>


int main(int argc, char **argv)
{
	t_env env;
	struct timeval tstart;
	
	gettimeofday(&tstart, NULL);
	if (argc < 2)
		display_help(EXIT_FAILURE);
	if (getuid())
		ft_error("ft_nmap requires root privileges!", EXIT_FAILURE);
	create_env(&env);
	parse_args(argc, argv, &env); 
	set_defaults(&env);
	display_header(&env);
	//NOW FOR THE ACTUAL THREAD BIT :( 
		
		// PPT

	return 0;
}