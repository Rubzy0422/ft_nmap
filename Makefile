# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: marvin <marvin@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/11/04 12:15:51 by rcoetzer          #+#    #+#              #
#    Updated: 2020/11/12 09:52:18 by marvin           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

ARGS		=	--file TODO --ports 0-10 --speedup 4
VALGRINDARGS = --leak-check=full

NAME		=	ft_nmap
CC			= 	gcc
FLAGS		=	-Wall -Wextra -g 

SRCDIR		=	src/
OBJDIR		=	obj/
REMOVE		= rm -rf

SRCFILES	=	main.c\
				ft_error.c\
				create_env.c\
				display_help.c\
				parse_args.c\
				utils.c\
				arg_err.c\
				binary_int.c\
				process_port.c\
				process_scan.c\
				process_file.c\
				process_speedup.c\
				process_ip.c\
				hostinfo.c\
				defaults.c\
				display_header.c\
				create_threads.c\
				create_socket.c\
				init_tcp.c\
				init_udp.c\
				csum.c\
				send_ack.c\
				send_fin.c\
				send_null.c\
				send_syn.c\
				send_udp.c\
				send_xmas.c\
				get_own_ip.c\
				recv_pack.c\
				ft_free.c

SRCS		=	$(addprefix $(SRCDIR), $(SRCFILES))
OBJS		=	$(addprefix $(OBJDIR), $(SRCFILES:.c=.o))

LIBFT_DIR	=	./libft/

LIBFT_LIB	=	$(LIBFT_DIR)libft.a
LIBS		=	$(LIBFT_LIB) -lpthread -lpcap -lm
INCS		=	-I inc -I $(LIBFT_DIR)/includes

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

$(LIBFT_LIB): #$(LIBFT_DIR)
	@make -C $(LIBFT_DIR)

$(LIBFT_DIR):
	# sudo apt-get install libpcap-dev
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
.PHONY: all clean fclean re