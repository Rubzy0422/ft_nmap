/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_port.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcoetzer <rcoetzer@student.wethinkcode.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/03 14:14:06 by rcoetzer          #+#    #+#             */
/*   Updated: 2020/11/07 14:48:45 by rcoetzer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_nmap.h>

int		ft_strisdigit(char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

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

static void add_ports_range(t_env *env, int start, int end)
{
	int i;

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

static void parse_port_part(t_env *env, char *part)
{
	char **splitted;
	int cnt;

	cnt = char_count(part, '-');
	if (cnt == 0)
	{
		env->params.ports[env->params.portcnt++] = ft_atoi(part);
		return;
	}
	if (!(splitted = ft_strsplit(part, '-')))
		ft_error("ft_nmap: can't malloc splitted ports", EXIT_FAILURE);
	if (!splitted[0] || cnt > 1 || !valid_port(splitted[0]) || (splitted[1] && !valid_port(splitted[1])))
		ft_error("Invalid port format", EXIT_FAILURE);
	if (splitted[1])
		add_ports_range(env, ft_atoi(splitted[0]), ft_atoi(splitted[1]));
	
	for (int i= 0; splitted[i]; i++)
		ft_free(splitted[i]);
	ft_free(splitted);
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


void optimize_ports(t_params *params)
{
	binary_sort(params->ports, params->portcnt); 
	params->portcnt = remove_duplicates(params->ports, params->portcnt);
	params->ports = realloc(params->ports, sizeof(int) * params->portcnt);
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
