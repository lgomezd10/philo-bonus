/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgomez-d <lgomez-d@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/30 19:48:52 by lgomez-d          #+#    #+#             */
/*   Updated: 2021/06/30 20:16:10 by lgomez-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int	load_arguments(t_data *data, int argc, char **argv)
{
	int	i;
	int	is_int;

	i = 1;
	is_int = 1;
	data->nbr_philos = ft_atol(argv[i++]);
	is_int = is_int && data->nbr_philos >= 0;
	data->time_to_die = ft_atol(argv[i++]);
	is_int = is_int && (int)data->time_to_die >= 0;
	data->time_to_eat = ft_atol(argv[i++]);
	is_int = is_int && (int)data->time_to_eat >= 0;
	data->time_to_sleep = ft_atol(argv[i]);
	is_int = is_int && (int)data->time_to_sleep >= 0;
	data->times_must_eat = -1;
	if (argc > 5)
	{
		data->times_must_eat = ft_atol(argv[i + 1]);
		is_int = is_int && data->times_must_eat >= 0;
	}
	return (is_int);
}

char *get_name(int nbr)
{
	char *name;

	name = malloc(sizeof(char) * 6);
	name[0] = '/';
	name[1] = 's';
	name[2] = 'e';
	name[3] = 'm';
	name[4] = '0' + nbr;
	name[5] = '\0';
	return (name);
}

int	load_forks(t_data *data)
{
	int	size_array;
	int	i;

	size_array = sizeof(t_fork) * data->nbr_philos;
	data->forks = (t_fork *)malloc(size_array);
	if (!data->forks)
		return (1);
	memset(data->forks, '\0', size_array);
	i = 0;
	while (i < data->nbr_philos)
	{

		data->forks[i].nbr = i + 1;
		data->forks[i].name = get_name(i + 1);
		data->forks[i].nbr_forks = data->nbr_philos;
		printf("Se va a crear el semaforo %s\n", data->forks[i].name);
		data->forks[i].sem = sem_open(data->forks[i].name, O_CREAT, 0644, 1);
		if 	(data->forks[i].sem == SEM_FAILED)
			show_error("Semaphore not work 1");
		i++;
	}
	printf("se va a crear el semaforo %s\n", SEM_PRINT);
	data->sem_print = sem_open(SEM_PRINT, O_CREAT, 0644, 1);
	if 	(data->sem_print == SEM_FAILED)
			show_error("Semaphore not work 2");
	

/*
	i = 0;
	while (i < data->nbr_philos)
	{
		sem_close(data->forks[i].sem);
		sem_unlink(data->forks[i].name);
		i++;
	}
	sem_close(data->sem_print);
	sem_unlink(SEM_PRINT);
	exit (0);
*/

	return (0);
}
