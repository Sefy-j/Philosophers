/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlopez-f <jlopez-f@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/11 13:14:52 by jlopez-f          #+#    #+#             */
/*   Updated: 2022/04/11 18:43:39 by jlopez-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*ft_starteating(void *arg)
{
	t_phlist	*lst;

	lst = (t_phlist *)arg;
	printf("EH Philo %d, time is %ld\n", lst->num, lst->time);
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
	ft_lstclear(&lst, args.num);
	return (0);
}
