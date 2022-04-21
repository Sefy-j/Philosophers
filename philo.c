/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlopez-f <jlopez-f@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/11 13:14:52 by jlopez-f          #+#    #+#             */
/*   Updated: 2022/04/21 20:21:52 by jlopez-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_dead(t_phlist lst)
{
	int	i;

	i = 0;
	while (i < lst.maxphil)
	{
		if (lst.dead == 1)
			return (1);
		i++;
		lst = *lst.next;
	}
	return (0);
}

void	ft_printstatus(t_phlist lst, char c)
{
	//int	dead;

	lst.time = ft_time(lst.timestart);
	//dead = ft_dead(lst);
	//if(!dead)
	//{
		if (c == 'f')
			printf(CYAN"%ld(ms): Philosopher %d has taken a fork\n"RESET, lst.time, lst.num);
		if (c == 'e')
			printf(GREEN"%ld(ms): Philosopher %d is eating\n"RESET, lst.time, lst.num);
		if (c == 's')
			printf(PURPLE"%ld(ms): Philosopher %d is sleeping\n"RESET, lst.time, lst.num);
		if (c == 't')
			printf(YELLOW"%ld(ms): Philosopher %d is thinking\n"RESET, lst.time, lst.num);
		if (c == 'd')
			printf(RED"%ld(ms): Philosopher %d died\n"RESET, lst.time, lst.num);
	//}
	return ;
}

void	*ft_starteating(void *arg)
{
	t_phlist	*lst;

	lst = (t_phlist *)arg;
	while (!ft_dead(*lst) && lst->repeats > 0)
	{
		lst->time = ft_time(lst->timestart);
		if (lst->time - lst->tmhungry > lst->tmdie)
		{
			lst->dead = 1;
			break ;
		}
		pthread_mutex_lock (&lst->mutexfork);
		lst->fork = 1;
		//pthread_mutex_lock (&lst->prev->mutexfork);
		if (lst->next->fork == 0)
		{
			pthread_mutex_lock (&lst->next->mutexfork);
			ft_printstatus(*lst, 'f');
			ft_printstatus(*lst, 'f');
			ft_printstatus(*lst, 'e');
			lst->tmhungry = ft_time(lst->timestart);
			if (lst->needrepeat)
				lst->repeats--;
			ft_usleep(lst->tmeat);
			pthread_mutex_unlock (&lst->next->mutexfork);
			ft_printstatus(*lst, 't');
			ft_usleep(lst->tmsleep);
		}
		lst->fork = 0;	
		//pthread_mutex_unlock (&lst->prev->mutexfork);
		pthread_mutex_unlock (&lst->mutexfork);
	}
	if (ft_dead(*lst))
		ft_printstatus(*lst, 'd');
	return (0);
}

/*void	leaks(void)
{
	system("leaks -q push_swap");
}*/

int	main(int argc, char **argv)
{
	t_arg	args;
	t_phlist	*lst;
	int		i;

	//atexit(leaks);
	if (argc < 5 || argc > 6)
		return (ft_errors(ERRARG, NULL));
	memset(&args, 0, sizeof(t_arg));
	if (ft_checkargv(argc, argv, &args))
		return (ERRGENERAL);
	lst = ft_createlist(args);
	i = 0;
	while (i < args.num)
	{
		pthread_create(&lst->hilo, NULL, &ft_starteating, (void *)lst);
		lst = lst->next;
		i++;
	}
	i = 0;
	lst = lst->next;
	while (i < args.num)
	{
		pthread_join(lst->hilo, NULL);
		lst = lst->next;
		i++;
	}
	i = 0;
	lst = lst->next;
	while (i < args.num)
	{
		pthread_mutex_destroy(&lst->mutexfork);
		lst = lst->next;
		i++;
	}
	ft_lstclear(&lst, args.num);
	return (0);
}

/*
void	*ft_starteating(void *arg)
{
	t_phlist	*lst;

	lst = (t_phlist *)arg;
	while (!ft_dead(*lst))
	{
		lst->time = ft_time(lst->timestart);
		if (lst->time - lst->tmhungry > lst->tmdie)
		{
			lst->dead = 1;
			break ;
		}
		if (lst->turn == 1)
		{
			pthread_mutex_lock (&lst->mutexfork);	
			ft_printstatus(*lst, 'f');		
			lst->next->fork = 1;
			lst->prev->fork = 1;
			pthread_mutex_unlock (&lst->mutexfork);
			ft_printstatus(*lst, 'e');
			lst->tmhungry = ft_time(lst->timestart);
			ft_usleep(lst->tmeat);
			lst->next->fork = 0;
			lst->prev->fork = 0;
			lst->turn = 0;
			lst->next->turn = 1;
			lst->prev->turn = 1;
			ft_printstatus(*lst, 't');
			ft_usleep(lst->tmsleep);
		}
	}
	ft_printstatus(*lst, 'd');
	return (0);
}*/
