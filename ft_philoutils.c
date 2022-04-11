/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_philoutils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlopez-f <jlopez-f@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/23 14:29:22 by jlopez-f          #+#    #+#             */
/*   Updated: 2022/04/11 18:41:44 by jlopez-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_errors(int error, char *name)
{
	if (error == ERRARG)
	{
		ft_putstr_fd("Error: Not correct number of arguments\n", 2);
		ft_putstr_fd("Use: ./philo <number_of_philosophers> <time_to_die>", 2);
		ft_putstr_fd(" <time_to_eat> <time_to_sleep>", 2);
		ft_putstr_fd(" <[number_of_times_each_philosopher_must_eat]>\n", 2);
	}
	if (error == ERREMPTARG)
		ft_putstr_fd("Error: Empty argument\n", 2);
	if (error == ERRNEGNUM)
		ft_putstr_fd("Error: Numbers can't be negative\n", 2);
	if (error == ERRNOTNUM)
	{
		ft_putstr_fd("Error: \n", 2);
		ft_putstr_fd(name, 2);
		ft_putstr_fd(" is not a number\n", 2);
	}
	return (ERRGENERAL);
}

time_t	ft_time(time_t start)
{
	struct timeval time;
	time_t	actual;

	gettimeofday(&time, NULL);
	actual = (time.tv_sec * 1000) + (time.tv_usec / 1000) - start;
	return (actual);
}