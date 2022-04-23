/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parsearg.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlopez-f <jlopez-f@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/23 18:01:54 by jlopez-f          #+#    #+#             */
/*   Updated: 2022/04/23 22:47:16 by jlopez-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	ft_copynums(int argc, char **argv, t_arg *args)
{
	int	i;

	i = 1;
	while (i < argc)
	{
		if (i == 1)
			args->num = ft_atoi(argv[i]);
		if (i == 2)
			args->tmdie = ft_atoi(argv[i]);
		if (i == 3)
			args->tmeat = ft_atoi(argv[i]);
		if (i == 4)
			args->tmsleep = ft_atoi(argv[i]);
		if (i == 5)
			args->repeats = ft_atoi(argv[i]);
		i++;
	}
	if (args->num < 0 || args->tmdie < 0 || args->tmeat < 0 \
	|| args->tmeat < 0 || args->tmsleep < 0 || args-> repeats < 0)
		return (ft_errors(ERRNEGNUM, NULL));
	if (i == 5)
		args->repeats = -1;
	return (0);
}

static int	ft_checkdigit(char *argv)
{
	int		i;
	int		len;

	i = 0;
	len = ft_strlen(argv);
	while (i < len)
	{
		if ((argv[i] == '-' || argv[i] == '+') && i == 0)
			i++;
		if (!ft_isdigit(argv[i]))
			return (ft_errors(ERRNOTNUM, argv));
		i++;
	}
	return (0);
}

int	ft_checkargv(int argc, char **argv, t_arg *args)
{
	int		i;

	i = 1;
	while (i < argc)
	{
		if (!(*argv[i]))
			ft_errors(ERREMPTARG, NULL);
		if (ft_checkdigit(argv[i]))
			return (ERRGENERAL);
		i++;
	}
	if (ft_copynums(argc, argv, args))
		return (ERRGENERAL);
	return (0);
}
