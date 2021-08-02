/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgomez-d <lgomez-d@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/30 19:48:52 by lgomez-d          #+#    #+#             */
/*   Updated: 2021/08/02 15:32:35 by lgomez-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int	load_arguments(t_data *data, int argc, char **argv)
{
	int	i;
	int	is_int;

	i = 1;
	is_int = 1;
	pthread_mutex_init(&data->shared.mutex_print, NULL);
	data->shared.pos = 4;
	if (data->shared.nbr_philos < 4)
		data->shared.pos = 3;
	data->shared.nbr_philos = ft_atol(argv[i++]);
	is_int = is_int && data->shared.nbr_philos >= 0;
	data->shared.time_to_die = ft_atol(argv[i++]);
	is_int = is_int && (int)data->shared.time_to_die >= 0;
	data->shared.time_to_eat = ft_atol(argv[i++]);
	is_int = is_int && (int)data->shared.time_to_eat >= 0;
	data->shared.time_to_sleep = ft_atol(argv[i]);
	is_int = is_int && (int)data->shared.time_to_sleep >= 0;
	data->times_must_eat = -1;
	if (argc > 5)
	{
		data->times_must_eat = ft_atol(argv[i + 1]);
		is_int = is_int && data->times_must_eat >= 0;
	}
	return (is_int);
}

int	load_philos(t_data *data)
{
	int	i;
	int	size_array;

	size_array = sizeof(t_philo) * data->shared.nbr_philos;
	data->philos = (t_philo *)malloc(size_array);
	if (!data->philos)
		return (1);
	memset((void *)data->philos, '\0', size_array);
	i = 0;
	while (i < data->shared.nbr_philos)
	{
		data->philos[i].nbr = i + 1;
		data->philos[i].times_must_eat = data->times_must_eat;
		data->philos[i].shared = &data->shared;
		data->philos[i].fork_right = &data->forks[i];
		if (data->shared.nbr_philos == 1)
			data->philos[i].fork_right = 0;
		else if (i == data->shared.nbr_philos - 1)
			data->philos[i].fork_left = &data->forks[0];
		else
			data->philos[i].fork_left = &data->forks[i + 1];
		i++;
	}
	return (0);
}

int	load_forks(t_data *data)
{
	int	size_array;
	int	i;

	size_array = sizeof(t_fork) * data->shared.nbr_philos;
	data->forks = (t_fork *)malloc(size_array);
	if (!data->forks)
		return (1);
	memset(data->forks, '\0', size_array);
	i = 0;
	while (i < data->shared.nbr_philos)
	{
		pthread_mutex_init(&data->forks[i].mutex, 0);
		data->forks[i].nbr = i + 1;
		i++;
	}
	return (0);
}
