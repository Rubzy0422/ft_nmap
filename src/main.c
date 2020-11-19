//Stack of scans :) 
#include <ft_nmap.h>

int main(int argc, char **argv)
{
	t_env env;
	struct timeval tstart;
	t_list *lst;
	
	gettimeofday(&tstart, NULL);
	if (getuid())
		ft_error("ft_nmap requires root privileges!", EXIT_FAILURE);
	if (argc < 2)
		display_help(EXIT_FAILURE);
	create_env(&env);
	parse_args(argc, argv, &env); 
	set_defaults(&env);
	display_header(&env);
	lst = init_scanlst(&env.params);
	if (env.params.thread_cnt == 0 )
		env.params.thread_cnt = THREADMAX;
	run_scans(lst, &env, tstart);
	return EXIT_SUCCESS;
}