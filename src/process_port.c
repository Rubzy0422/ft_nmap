/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_port.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcoetzer <rcoetzer@student.wethinkcode.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/03 14:14:06 by rcoetzer          #+#    #+#             */
/*   Updated: 2020/11/23 08:45:45 by rcoetzer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_nmap.h>


int valid_port(char *port)
{
	int atoied;

	if (!ft_strisdigit(port))
		return (0);
	while (port[0] == '0')
		port++;
	if (ft_strlen(port) > 5)
		return (0);
	if ((atoied = ft_atoi(port)) < 0 || atoied > 65535)
		return (0);
	return (1);
}

void add_ports_range(t_env *env, char *split0, char *split1)
{
	int i;
	int start;
	int end;
	
	start = ft_atoi(split0);
	end = ft_atoi(split1);
	if (!start || !end)
		ft_error("Invalid Port range", EXIT_FAILURE);
	if (start > end)
	{
		i = start;
		start = end;
		end = i;
	}
	i = start;
	while (i <= end)
	{
		env->params.ports[env->params.portcnt++] = i;
		i++;
	}
}

void parse_port_part(t_env *env, char *part)
{
	char **splitted;
	int cnt;
	int i;

	cnt = char_count(part, '-');
	if (cnt == 0)
	{
		if (ft_strisdigit(part))
			env->params.ports[env->params.portcnt++] = ft_atoi(part);
		else 
			ft_error("Invalid port format", EXIT_FAILURE);	
		return;
	}
	if (!(splitted = ft_strsplit(part, '-')))
		ft_error("ft_nmap: can't malloc splitted ports", EXIT_FAILURE);

	if (splitcnt(splitted) == 2)
	{
		if (cnt > 1 || !valid_port(splitted[0]) || !valid_port(splitted[1]))
			ft_error("Invalid port format", EXIT_FAILURE);
		add_ports_range(env, splitted[0], splitted[1]);
	}
	else 
		ft_error("Invalid port format", EXIT_FAILURE);
	i= 0;
	while(splitted[i])
	{
		ft_free(splitted[i]);
		i++;
	}
	ft_free(splitted);
}

void	process_port(char **argv, int argc, int i, t_env *env)
{
	if (i + 1 < argc)
	{
		parse_ports(env, argv[i + 1]);
		if (env->params.portcnt > MAX_PORTS)
		{
			fprintf(stderr, "Too many Ports spesified!\n");
			ft_free(env->params.ports);
			exit(EXIT_FAILURE);
		}
		else
			optimize_ports(&env->params);
	}
	else
		arg_err("--port");
}
