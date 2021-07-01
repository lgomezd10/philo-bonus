/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgomez-d <lgomez-d@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/30 19:48:20 by lgomez-d          #+#    #+#             */
/*   Updated: 2021/06/30 20:13:11 by lgomez-d         ###   ########.fr       */
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
#include <sys/types.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/wait.h>


typedef struct s_fork
{
	sem_t	*sem;
	int		nbr;
	char	*name;
	pthread_t	id_thread;
	int		capture;
}	t_fork;

typedef struct s_data
{
	int				nbr;
	t_fork			*forks;
	int				someone_is_dead;
	int				nbr_philos;
	useconds_t		time_to_die;
	useconds_t		time_to_eat;
	useconds_t		time_to_sleep;	
	int			times_must_eat;
	time_t			last_meal;
	time_t			init_time;
}	t_data;

int		ft_atol(const char *nptr);
time_t	get_time(void);
time_t	time_spent(t_data *data);
void	clean_all(t_data *data);
int		load_arguments(t_data *data, int argc, char **argv);
int		load_forks(t_data *data);
int		load_philos(t_data *data);
int		throw_threads(t_data *data);
void	print_change(t_data *data, char *action, time_t time);
void	print_dead(t_data *data);
void	*run_thread(void *data_philo);
int		run_eat(t_data *data);
int		run_sleep(t_data *data);

char	*ft_strjoin(char const *s1, char const *s2);

#endif