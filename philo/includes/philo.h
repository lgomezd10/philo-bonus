/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgomez-d <lgomez-d@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/30 19:48:20 by lgomez-d          #+#    #+#             */
/*   Updated: 2021/07/26 15:43:41 by lgomez-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H
# include <unistd.h>
# include <sys/time.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <pthread.h>

typedef struct s_shared
{
	int				someone_is_dead;
	int				nbr_philos;
	useconds_t		time_to_die;
	useconds_t		time_to_eat;
	useconds_t		time_to_sleep;
	pthread_mutex_t	mutex_print;
}	t_shared;

typedef struct s_fork
{
	pthread_mutex_t	mutex;
	int				nbr;
}	t_fork;

typedef struct s_philo
{
	int				nbr;
	int				times_must_eat;
	pthread_t		id_thread;
	t_fork			*fork_left;
	t_fork			*fork_right;
	pthread_mutex_t	*mutex1;
	pthread_mutex_t	*mutex2;
	t_shared		*shared;
	time_t			last_meal;
	time_t			init_time;
	int				end;
}	t_philo;

typedef struct s_data
{
	t_philo		*philos;
	t_fork		*forks;
	t_shared	shared;
	int			times_must_eat;
}	t_data;

int		ft_atol(const char *nptr);
time_t	get_time(void);
time_t	time_spent(t_philo *philo);
void	clean_all(t_data *data);
int		load_arguments(t_data *data, int argc, char **argv);
int		load_forks(t_data *data);
int		load_philos(t_data *data);
int		throw_threads(t_data *data);
void	print_change(t_philo *philo, char *action, time_t time);
void	print_dead(t_philo *philo);
void	*run_thread(void *data_philo);

#endif