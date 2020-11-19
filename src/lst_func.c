/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_func.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcoetzer <rcoetzer@student.wethinkcode.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/12 20:21:44 by rcoetzer          #+#    #+#             */
/*   Updated: 2020/11/12 21:33:33 by rcoetzer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_nmap.h>

void ft_lstappend(t_list *lst, t_list *new)
{
	if (!lst)
		return ;
	while (lst->next)
		lst = lst->next;
	lst->next = new;
}

t_list *ft_lstpop(t_list **lst)
{
	t_list * tmp = *lst;
	if (tmp)
	{
		*lst = tmp->next;
	}
	return tmp;
}
