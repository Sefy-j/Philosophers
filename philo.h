/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlopez-f <jlopez-f@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/13 13:34:47 by jlopez-f          #+#    #+#             */
/*   Updated: 2022/04/21 20:19:02 by jlopez-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <pthread.h>
# include <sys/types.h>
# include <sys/time.h>
# include <limits.h>

typedef struct s_list
{
	pthread_t		hilo;
	int				fork;
	int				num;
	int				maxphil;
	int				tmdie;
	int				tmeat;
	int				tmsleep;
	int				repeats;
	int				needrepeat;
	int				dead;
	int				noprint;
	int				turn;
	time_t			timestart;
	time_t			time;
	time_t			tmhungry;
	pthread_mutex_t	mutexfork;
	struct s_list	*next;
	struct s_list	*prev;
}	t_phlist;

typedef struct s_arg
{
	int	num;
	int	tmdie;
	int	tmeat;
	int	tmsleep;
	int	repeats;
}	t_arg;

enum {
	ERRGENERAL = 1,
	ERRARG = 2,
	ERREMPTARG = 3,
	ERRNEGNUM = 4,
	ERRNOTNUM = 5
};

# define RESET				"\x1b[0m"
# define WHITE				"\x1b[1m"
# define GRAY				"\x1b[2m"
# define BLACK				"\x1b[30m"
# define RED				"\x1b[31m"
# define GREEN				"\x1b[32m"
# define YELLOW				"\x1b[33m"
# define BLUE				"\x1b[34m"
# define PURPLE				"\x1b[35m"
# define CYAN				"\x1b[36m"

int		ft_checkargv(int argc, char **argv, t_arg *args);
t_phlist	*ft_createlist(t_arg args);
time_t	ft_time(time_t start);
void	ft_lstclear(t_phlist **lst, int num);
int		ft_isdigit(int c);
int		ft_atoi(const char *str);
size_t	ft_strlen(const char *str);
int		ft_errors(int error, char *name);
void	ft_putstr_fd(char *s, int fd);
void	ft_usleep(time_t time);

#endif
