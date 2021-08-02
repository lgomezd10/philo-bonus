/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgomez-d <lgomez-d@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/05 19:47:11 by lgomez-d          #+#    #+#             */
/*   Updated: 2021/08/02 17:15:29 by lgomez-d         ###   ########.fr       */
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

char	*get_name(int nbr)
{
	char	*name;

	name = (char *)malloc(sizeof(char) * 6);
	if (!name)
		show_error("Malloc error");
	name[0] = '/';
	name[1] = 's';
	name[2] = 'e';
	name[3] = 'm';
	name[4] = '0' + nbr;
	name[5] = '\0';
	return (name);
}

static void	open_sem(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nbr_philos)
	{
		data->forks[i].nbr = i + 1;
		data->forks[i].name = get_name(i + 1);
		data->forks[i].sem = sem_open(data->forks[i].name, O_CREAT, 0644, 1);
		if (data->forks[i].sem == SEM_FAILED)
			show_error("Semaphore error");
		i++;
	}
	data->sem_print = sem_open(SEM_PRINT, O_CREAT, 0644, 1);
	if (data->sem_print == SEM_FAILED)
		show_error("Semaphore error");
	data->sem_catch_forks = sem_open(SEM_CATCH, O_CREAT, 0644, 1);
	if (data->sem_catch_forks == SEM_FAILED)
		show_error("Semaphore error");
}

static void	unlink_sem(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nbr_philos)
	{
		sem_close(data->forks[i].sem);
		sem_unlink(data->forks[i].name);
		i++;
	}
	sem_close(data->sem_print);
	sem_unlink(SEM_PRINT);
	sem_close(data->sem_catch_forks);
	sem_unlink(SEM_CATCH);
}

int	load_forks(t_data *data)
{
	int	size_array;

	size_array = sizeof(t_fork) * data->nbr_philos;
	data->forks = (t_fork *)malloc(size_array);
	if (!data->forks)
		return (1);
	memset(data->forks, '\0', size_array);
	open_sem(data);
	unlink_sem(data);
	open_sem(data);
	return (0);
}
