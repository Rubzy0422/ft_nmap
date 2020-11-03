/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_file.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcoetzer <rcoetzer@student.wethinkcode.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/23 11:15:39 by rcoetzer          #+#    #+#             */
/*   Updated: 2020/11/03 18:25:15 by rcoetzer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_nmap.h>

void	process_file(char **argv, int argc, int i, t_env *env)
{
	char	*filename;
	int		fd;
	if (i + 1 < argc)
		filename = argv[i + 1];
	else if (i + 1 >= argc)
		arg_err("--file");

	FILE *file = fopen(filename, "r");
	if (!file)
		ft_error("File", -1);
	fd = fileno(file);
	if (fd <= 0)
		ft_error("fileno", -1);

	while (get_next_line(fd, &env->buff) > 0)
	{
		if (env->hostcnt < MAX_HOST)
			get_host(env, ft_strdup(env->buff));
		else
		{
			free(env->buff);
			ft_error("MAX HOST Exceeded", 0);
		}
		free(env->buff);
	}
	fclose(file);
}
