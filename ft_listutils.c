/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_listutils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlopez-f <jlopez-f@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/22 17:02:43 by jlopez-f          #+#    #+#             */
/*   Updated: 2022/04/11 18:42:58 by jlopez-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	ft_lstdelone(t_phlist *lst)
{
	if (!lst)
		return ;
	free(lst);
}

void	ft_lstclear(t_phlist **lst, int num)
{
	t_phlist	*tmplist;
	int	i;

	if (!lst)
		return ;
	i = 0;
	while (i < num)
	{
		tmplist = (*lst)->next;
		ft_lstdelone(*lst);
		*lst = tmplist;
		i++;
	}
	*lst = NULL;
}

static t_phlist	*ft_lstnew(int num, t_arg args)
{
	t_phlist	*nelem;

	nelem = (t_phlist *)malloc(sizeof(t_phlist));
	if (!nelem)
		return (NULL);
	nelem->fork = 0 * args.num * num;
	nelem->num = num;
	nelem->maxphil = args.num;
	nelem->tmdie = args.tmdie;
	nelem->tmeat = args.tmeat;
	nelem->tmsleep = args.tmsleep;
	nelem->repeats = args.repeats;
	nelem->dead = 0;
	nelem->time = ft_time(0);
	nelem->next = NULL;
	return (nelem);
}

t_phlist	*ft_createlist(t_arg args)
{
	t_phlist	*lst;
	t_phlist	*temp;
	t_phlist	*first;
	int		i;

	i = 1;
	lst = ft_lstnew(i, args);
	if (!lst)
		return (NULL);
	first = lst;
	while (i < args.num)
	{
		temp = ft_lstnew(i + 1, args);
		lst->next = temp;
		temp->prev = lst;
		lst = lst->next;
		i++;
	}
	first->prev = lst;
	lst->next = first;
	lst = lst->next;
	return (lst);
}