/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_checkandprint.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlopez-f <jlopez-f@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/25 13:29:17 by jlopez-f          #+#    #+#             */
/*   Updated: 2022/04/25 13:29:17 by jlopez-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_alleat(t_phlist lst, pthread_mutex_t	*mutexeat)
{
	int	i;
	int	eat;

	i = 0;
	eat = 0;
	pthread_mutex_lock (mutexeat);
	while (i < lst.maxphil)
	{
		if (lst.repeats <= 0)
			eat++;
		i++;
		lst = *lst.next;
	}
	pthread_mutex_unlock (mutexeat);
	if (eat == lst.maxphil)
		return (1);
	else
		return (0);
}

void	ft_printstatus(t_phlist lst, char c, int *dead, pthread_mutex_t	*mutex)
{
	lst.time = ft_time(lst.timestart);
	pthread_mutex_lock (&mutex[DEAD]);
	if (!*dead && !ft_alleat(lst, &mutex[EAT]))
	{
		if (c == 'f')
			printf(CYAN"%ld(ms): Philosopher %d has taken a fork\n"RESET, \
			lst.time, lst.num);
		if (c == 'e')
			printf(GREEN"%ld(ms): Philosopher %d is eating\n"RESET, \
			lst.time, lst.num);
		if (c == 's')
			printf(PURPLE"%ld(ms): Philosopher %d is sleeping\n"RESET, \
			lst.time, lst.num);
		if (c == 't')
			printf(YELLOW"%ld(ms): Philosopher %d is thinking\n"RESET, \
			lst.time, lst.num);
	}
	pthread_mutex_unlock (&mutex[DEAD]);
	return ;
}

int	ft_checkdead(t_phlist *lst, int *dead, pthread_mutex_t *mutex)
{
	lst->time = ft_time(lst->timestart);
	pthread_mutex_lock (&mutex[DEAD]);
	if (lst->time - lst->tmhungry > lst->tmdie)
		(*dead)++;
	if (*dead)
	{
		if (*dead == 1)
			printf(RED"%ld(ms): Philosopher %d died\n"RESET, \
			lst->time, lst->num);
		(*dead)++;
		pthread_mutex_unlock (&mutex[DEAD]);
		return (1);
	}
	pthread_mutex_unlock (&mutex[DEAD]);
	return (0);
}
