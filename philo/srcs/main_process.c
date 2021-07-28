/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_process.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgomez-d <lgomez-d@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/30 20:02:30 by lgomez-d          #+#    #+#             */
/*   Updated: 2021/07/27 18:45:41 by lgomez-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

static void	wait_for_dead(t_data *data)
{
	int		i;
	t_philo	*philo;
	int		end_all;

	usleep(data->shared.time_to_die * 1000);
	end_all = 0;
	while (!data->shared.someone_is_dead && !end_all)
	{
		i = 0;
		end_all = 1;
		while (i < data->shared.nbr_philos)
		{
			philo = &data->philos[i];
			if (!philo->end)
			{
				end_all = 0;
				if (time_spent(philo) > data->shared.time_to_die + 2)
				{
					print_dead(philo);
					pthread_detach(philo->id_thread);
				}
			}
			i++;
		}		
	}
}

int	throw_threads(t_data *data)
{
	int		i;
	int		has_error;
	t_philo	*philo;

	i = 0;
	while (i < data->shared.nbr_philos)
	{
		philo = &data->philos[i];
		has_error = pthread_create(&philo->id_thread, NULL, run_thread, philo);
		if (has_error)
			return (1);
		i++;
	}
	wait_for_dead(data);
	i = 0;
	while (i < data->shared.nbr_philos)
	{
		philo = &data->philos[i];
		pthread_join(philo->id_thread, NULL);
		i++;
	}
	clean_all(data);
	return (0);
}

void	print_change(t_philo *philo, char *action, time_t time)
{
	time_t	time_action;

	if (!philo->shared->someone_is_dead)
	{
		pthread_mutex_lock(&philo->shared->mutex_print);
		time_action = get_time() - philo->init_time;
		if (!philo->shared->someone_is_dead)
			printf("\x1b[37m%ld %d %s real time: %ld\n", time_action, philo->nbr, action, time);
		pthread_mutex_unlock(&philo->shared->mutex_print);
	}
}

void	print_dead(t_philo *philo)
{
	time_t	time_action;

	if (!philo->shared->someone_is_dead)
	{
		pthread_mutex_lock(&philo->shared->mutex_print);
		if (!philo->shared->someone_is_dead)
		{
			time_action = get_time() - philo->init_time;
			printf("%ld %d died\n", time_action, philo->nbr);
			philo->shared->someone_is_dead = 1;
		}
		pthread_mutex_unlock(&philo->shared->mutex_print);
	}
}
