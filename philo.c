/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlopez-f <jlopez-f@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/11 13:14:52 by jlopez-f          #+#    #+#             */
/*   Updated: 2022/06/02 17:19:46 by jlopez-f         ###   ########.fr       */
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
	ft_usleep(lst->tmeat, lst, dead, mutex);
	pthread_mutex_lock (&lst->next->mutexfork);
	lst->next->fork = 0;
	pthread_mutex_unlock (&lst->next->mutexfork);
	pthread_mutex_lock (&lst->mutexfork);
	lst->fork = 0;
	pthread_mutex_unlock (&lst->mutexfork);
	ft_printstatus(*lst, 's', dead, mutex);
	ft_usleep(lst->tmsleep, lst, dead, mutex);
	ft_printstatus(*lst, 't', dead, mutex);
	ft_usleep((lst->tmdie - (lst->tmeat + lst->tmsleep)) / 2, lst, dead, mutex);
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
	if (lst->num % 2 != 0)
		ft_usleep(20, lst, &dead, mutex);
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
