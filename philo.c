/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlopez-f <jlopez-f@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/11 13:14:52 by jlopez-f          #+#    #+#             */
/*   Updated: 2022/04/25 13:36:49 by jlopez-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	ft_forkstaken(t_phlist *lst, int *dead, pthread_mutex_t *mutex)
{
	ft_printstatus(*lst, 'f', dead, mutex);
	ft_printstatus(*lst, 'f', dead, mutex);
	ft_printstatus(*lst, 'e', dead, mutex);
	lst->tmhungry = ft_time(lst->timestart);
	if (lst->needrepeat)
	{
		pthread_mutex_lock (&mutex[EAT]);
		lst->repeats--;
		pthread_mutex_unlock (&mutex[EAT]);
	}
	ft_usleep(lst->tmeat);
	pthread_mutex_lock (&lst->next->mutexfork);
	lst->next->fork = 0;
	pthread_mutex_unlock (&lst->next->mutexfork);
	pthread_mutex_lock (&lst->mutexfork);
	lst->fork = 0;
	pthread_mutex_unlock (&lst->mutexfork);
	ft_printstatus(*lst, 's', dead, mutex);
	ft_usleep(lst->tmsleep);
	ft_printstatus(*lst, 't', dead, mutex);
	ft_usleep((lst->tmdie - (lst->tmeat + lst->tmsleep)) / 2);
}

static void	ft_eating(t_phlist *lst, int *dead, pthread_mutex_t *mutex)
{
	pthread_mutex_lock (&lst->mutexfork);
	if (lst->fork == 0)
	{
		lst->fork = 1;
		pthread_mutex_unlock (&lst->mutexfork);
		pthread_mutex_lock (&lst->next->mutexfork);
		if (lst->next->fork == 0)
		{
			lst->next->fork = 1;
			pthread_mutex_unlock (&lst->next->mutexfork);
			ft_forkstaken(lst, dead, mutex);
		}
		else
		{
			pthread_mutex_unlock (&lst->next->mutexfork);
			pthread_mutex_lock (&lst->mutexfork);
			lst->fork = 0;
			pthread_mutex_unlock (&lst->mutexfork);
		}
	}
	else
		pthread_mutex_unlock (&lst->mutexfork);
}

static void	*ft_starteating(void *arg)
{
	t_phlist				*lst;
	static int				dead;
	static pthread_mutex_t	mutex[2] = {
		PTHREAD_MUTEX_INITIALIZER,
		PTHREAD_MUTEX_INITIALIZER
	};

	lst = (t_phlist *)arg;
	while (!ft_alleat(*lst, &mutex[EAT]))
	{
		if (ft_checkdead(lst, &dead, mutex))
			break ;
		ft_eating(lst, &dead, mutex);
	}
	return (0);
}

static void	ft_threads(t_arg args, t_phlist *lst)
{
	int	i;

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
}

int	main(int argc, char **argv)
{
	t_arg		args;
	t_phlist	*lst;
	int			i;

	if (argc < 5 || argc > 6)
		return (ft_errors(ERRARG, NULL));
	memset(&args, 0, sizeof(t_arg));
	if (ft_checkargv(argc, argv, &args))
		return (ERRGENERAL);
	lst = ft_createlist(args);
	ft_threads(args, lst);
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

/*void	leaks(void)
{
	system("leaks -q push_swap");
}*/
//atexit(leaks);

/*-----------------TURNOS_--------------------------------------------------
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

/*----------------------NORMAL_---------------------------------------
void	*ft_starteating(void *arg)
{
	t_phlist				*lst;
	static int				dead;
	static pthread_mutex_t	mutexdead = PTHREAD_MUTEX_INITIALIZER;
	static pthread_mutex_t	mutexeat = PTHREAD_MUTEX_INITIALIZER;

	lst = (t_phlist *)arg;
	
	while (!ft_alleat(*lst, &mutexeat))
	{
		lst->time = ft_time(lst->timestart);
		pthread_mutex_lock (&mutexdead);
		if (lst->time - lst->tmhungry > lst->tmdie)
			dead++;
		if (dead)
		{
			if (dead == 1)
				printf(RED"%ld(ms): Philosopher %d died\n"RESET, lst->time, lst->num);
			dead++;
			pthread_mutex_unlock (&mutexdead);
			break;
		}
		pthread_mutex_unlock (&mutexdead);
		pthread_mutex_lock (&lst->mutexfork);
		if (lst->fork == 0)
		{
			lst->fork = 1;
			pthread_mutex_unlock (&lst->mutexfork);
			pthread_mutex_lock (&lst->next->mutexfork);
			if (lst->next->fork == 0)			
			{
				lst->next->fork = 1;
				pthread_mutex_unlock (&lst->next->mutexfork);
				ft_printstatus(*lst, 'f', &dead, &mutexdead, &mutexeat);
				ft_printstatus(*lst, 'f', &dead, &mutexdead, &mutexeat);
				ft_printstatus(*lst, 'e', &dead, &mutexdead, &mutexeat);
				lst->tmhungry = ft_time(lst->timestart);
				if (lst->needrepeat)
				{
					pthread_mutex_lock (&mutexeat);
					lst->repeats--;
					pthread_mutex_unlock (&mutexeat);
				}
				ft_usleep(lst->tmeat);
				pthread_mutex_lock (&lst->next->mutexfork);
				lst->next->fork = 0;
				pthread_mutex_unlock (&lst->next->mutexfork);
				pthread_mutex_lock (&lst->mutexfork);
				lst->fork = 0;
				pthread_mutex_unlock (&lst->mutexfork);
				ft_printstatus(*lst, 's', &dead, &mutexdead, &mutexeat);
				ft_usleep(lst->tmsleep);
				ft_printstatus(*lst, 't', &dead, &mutexdead, &mutexeat);
			}
			else
			{
				pthread_mutex_unlock (&lst->next->mutexfork);
				pthread_mutex_lock (&lst->mutexfork);
				lst->fork = 0;
				pthread_mutex_unlock (&lst->mutexfork);
			}
		}
		else
			pthread_mutex_unlock (&lst->mutexfork);
	}
	return (0);
}*/

/*-------------------------------ODD EVEN---------------------------------
void	ft_evenphilo(t_phlist *lst, int *dead, pthread_mutex_t
	*mutexdead, pthread_mutex_t	*mutexeat)
{
	pthread_mutex_lock (&lst->next->mutexfork);
	if (lst->next->fork == 0)
	{
		lst->next->fork = 1;
		pthread_mutex_unlock (&lst->next->mutexfork);
		pthread_mutex_lock (&lst->mutexfork);
		if (lst->fork == 0)			
		{
			lst->fork = 1;
			pthread_mutex_unlock (&lst->mutexfork);
			ft_printstatus(*lst, 'f', dead, mutexdead, mutexeat);
			ft_printstatus(*lst, 'f', dead, mutexdead, mutexeat);
			ft_printstatus(*lst, 'e', dead, mutexdead, mutexeat);
			lst->tmhungry = ft_time(lst->timestart);
			if (lst->needrepeat)
			{
				pthread_mutex_lock (mutexeat);
				lst->repeats--;
				pthread_mutex_unlock (mutexeat);
			}
			ft_usleep(lst->tmeat);
			pthread_mutex_lock (&lst->mutexfork);
			lst->fork = 0;
			pthread_mutex_unlock (&lst->mutexfork);
			pthread_mutex_lock (&lst->next->mutexfork);
			lst->next->fork = 0;
			pthread_mutex_unlock (&lst->next->mutexfork);
			ft_printstatus(*lst, 's', dead, mutexdead, mutexeat);
			ft_usleep(lst->tmsleep);
			ft_printstatus(*lst, 't', dead, mutexdead, mutexeat);
		}
		else
		{
			pthread_mutex_unlock (&lst->mutexfork);
			pthread_mutex_lock (&lst->next->mutexfork);
			lst->next->fork = 0;
			pthread_mutex_unlock (&lst->next->mutexfork);
		}
	}
	else
		pthread_mutex_unlock (&lst->next->mutexfork);
	return ;
}

void	ft_oddphilo(t_phlist *lst, int *dead, pthread_mutex_t
	*mutexdead, pthread_mutex_t	*mutexeat)
{
	pthread_mutex_lock (&lst->mutexfork);
	if (lst->fork == 0)
	{
		lst->fork = 1;
		pthread_mutex_unlock (&lst->mutexfork);
		pthread_mutex_lock (&lst->next->mutexfork);
		if (lst->next->fork == 0)			
		{
			lst->next->fork = 1;
			pthread_mutex_unlock (&lst->next->mutexfork);
			ft_printstatus(*lst, 'f', dead, mutexdead, mutexeat);
			ft_printstatus(*lst, 'f', dead, mutexdead, mutexeat);
			ft_printstatus(*lst, 'e', dead, mutexdead, mutexeat);
			lst->tmhungry = ft_time(lst->timestart);
			if (lst->needrepeat)
			{
				pthread_mutex_lock (mutexeat);
				lst->repeats--;
				pthread_mutex_unlock (mutexeat);
			}
			ft_usleep(lst->tmeat);
			pthread_mutex_lock (&lst->next->mutexfork);
			lst->next->fork = 0;
			pthread_mutex_unlock (&lst->next->mutexfork);
			pthread_mutex_lock (&lst->mutexfork);
			lst->fork = 0;
			pthread_mutex_unlock (&lst->mutexfork);
			ft_printstatus(*lst, 's', dead, mutexdead, mutexeat);
			ft_usleep(lst->tmsleep);
			ft_printstatus(*lst, 't', dead, mutexdead, mutexeat);
		}
		else
		{
			pthread_mutex_unlock (&lst->next->mutexfork);
			pthread_mutex_lock (&lst->mutexfork);
			lst->fork = 0;
			pthread_mutex_unlock (&lst->mutexfork);
		}
	}
	else
		pthread_mutex_unlock (&lst->mutexfork);
	return ;
}

void	*ft_starteating(void *arg)
{
	t_phlist				*lst;
	static int				dead;
	static pthread_mutex_t	mutexdead = PTHREAD_MUTEX_INITIALIZER;
	static pthread_mutex_t	mutexeat = PTHREAD_MUTEX_INITIALIZER;

	lst = (t_phlist *)arg;
	
	while (!ft_alleat(*lst, &mutexeat))
	{
		lst->time = ft_time(lst->timestart);
		pthread_mutex_lock (&mutexdead);
		if (lst->time - lst->tmhungry > lst->tmdie)
			dead++;
		if (dead)
		{
			if (dead == 1)
				printf(RED"%ld(ms): Philosopher %d died\n"RESET, lst->time, lst->num);
			dead++;
			pthread_mutex_unlock (&mutexdead);
			break;
		}
		pthread_mutex_unlock (&mutexdead);
		if (lst->num % 2 == 0)
			ft_evenphilo(lst, &dead, &mutexdead, &mutexeat);
		else
			ft_oddphilo(lst, &dead, &mutexdead, &mutexeat);
		
	}
	return (0);
}*/

/*
	5 600 150 150
	4 410 200 200
	100 800 200 200
	105 800 200 200
	200 800 200 200
*/