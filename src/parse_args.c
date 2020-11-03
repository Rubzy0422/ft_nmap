/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcoetzer <rcoetzer@student.wethinkcode.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/03 12:38:55 by rcoetzer          #+#    #+#             */
/*   Updated: 2020/11/03 18:28:29 by rcoetzer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_nmap.h>

void parse_args(int argc, char **argv, t_env *env)
{
	int		i;
	
	i = 0;
	while (++i < argc)
	{
		if (!ft_strcmp(argv[i],"--help"))
			display_help(EXIT_SUCCESS);
		else if (!ft_strcmp(argv[i],"--ports"))
			process_port(argv, argc, i, env);
		else if (!ft_strcmp(argv[i],"--ip"))
			process_ip(argv, argc, i, env);
		else if (!ft_strcmp(argv[i],"--file"))
			process_file(argv, argc, i, env);
		else if (!ft_strcmp(argv[i],"--speedup"))
			process_speedup(argv, argc, i, env);
		else if (!ft_strcmp(argv[i],"--scan"))
			process_scan(argv, argc, i, env);
		else if (!ft_strncmp(argv[i], "--", 2))
		{
			fprintf(stderr, "Unknown argument %s\n", argv[i]);
			exit(EXIT_FAILURE);
		}
	}
}