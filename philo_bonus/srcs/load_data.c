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

	name = malloc(sizeof(char) * 5);
	name[0] = 's';
	name[1] = 'e';
	name[2] = 'm';
	name[3] = '0' + nbr;
	name[4] = '\0';
	return (name);
}

int	load_forks(t_data *data)
{
	int	size_array;
	int	i;
	char *name;

	size_array = sizeof(t_fork) * data->nbr_philos;
	data->forks = (t_fork *)malloc(size_array);
	if (!data->forks)
		return (1);
	memset(data->forks, '\0', size_array);
	i = 1;
	while (i <= data->nbr_philos)
	{

		data->forks[i].nbr = i;
		data->forks[i].name = get_name(i);
		data->forks[i].sem = sem_open(data->forks[i].name, O_CREAT);		
		i++;
	}
	return (0);
}
