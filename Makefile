# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rcoetzer <rcoetzer@student.wethinkcode.    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/11/04 12:15:51 by rcoetzer          #+#    #+#              #
#    Updated: 2020/11/23 09:13:00 by rcoetzer         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

ARGS		=	--file TODO --ports 0-10 --speedup 4
VALGRINDARGS = --leak-check=full

NAME		=	ft_nmap
CC			= 	gcc
FLAGS		=	-Wall -Wextra -g 

SRCDIR		=	src/
OBJDIR		=	obj/
REMOVE		=	rm -rf

SRCFILES	=	arg_err.c\
				binary_int.c\
				create_env.c\
				csum.c\
				defaults.c\
				display_header.c\
				display_help.c\
				ft_error.c\
				ft_free.c\
				get_host.c\
				get_own_ip.c\
				init_scanlst.c\
				init_tcp.c\
				init_udp.c\
				lst_func.c\
				main.c\
				packet_handler.c\
				parse_args.c\
				process_file.c\
				process_ip.c\
				process_port.c\
				process_ports.c\
				process_scan.c\
				process_speedup.c\
				send_ack.c\
				send_fin.c\
				send_null.c\
				send_syn.c\
				send_udp.c\
				send_xmas.c\
				utils.c\
				run_scans.c\
				send_recv_pack.c\
				display_data.c\
				time.c\
				ft_realloc.c\
				debug_print.c\
				malzero.c\
				splitcnt.c

SRCS		=	$(addprefix $(SRCDIR), $(SRCFILES))
OBJS		=	$(addprefix $(OBJDIR), $(SRCFILES:.c=.o))

LIBFT_DIR	=	./libft/

LIBFT_LIB	=	$(LIBFT_DIR)libft.a
LIBS		=	$(LIBFT_LIB) -lpthread -lpcap
INCS		=	-I inc -I $(LIBFT_DIR)/includes -I libpcap

############################################## COLORS 

Black =  \u001b[30;1m
Red =  \u001b[31;1m
Green =  \u001b[32;1m
Yellow =  \u001b[33;1m
Blue =  \u001b[34;1m
Magenta =  \u001b[35;1m
Cyan =  \u001b[36;1m
White =  \u001b[37;1m
Reset = \u001b[0m


all: $(NAME)

$(NAME): $(LIBFT_LIB) $(OBJDIR) $(OBJS)
	@$(CC) -lm $(FLAGS) -o $(NAME) $(OBJS) $(LIBS)
	@echo COMPILED $(NAME)

$(LIBFT_LIB): $(LIBFT_DIR)
	@make -C $(LIBFT_DIR)

$(LIBFT_DIR):
	sudo apt-get install libpcap-dev
	@git clone http://github.com/rubzy0422/libft.git 

$(OBJDIR):
	@mkdir -p $(OBJDIR)
	@mkdir -p $(dir $(OBJS))

$(OBJDIR)%.o : $(SRCDIR)%.c | $(OBJDIR)
	@$(CC) $(FLAGS) -c $< -o $@ $(INCS)
	@echo COMPILED $@

clean:
	@make -C $(LIBFT_DIR) clean
	@$(REMOVE) $(OBJDIR)

fclean:		clean
	@make -C $(LIBFT_DIR) fclean
	@$(REMOVE) $(NAME)

re:	fclean all

test: all 
	sudo valgrind $(VALGRINDARGS) ./$(NAME) $(ARGS)

# Reason: 1024 ports will do 6 scans and will time out 1 scan at around 1 second
# thus you will wait (1024 *6) seconds .. thus (6144 s /102.4 minutes / 1.70 Hours)

test1: all 
	sudo ./$(NAME) --speedup 250 
.PHONY: all clean fclean re