/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_process.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgomez-d <lgomez-d@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/30 20:02:30 by lgomez-d          #+#    #+#             */
/*   Updated: 2021/06/30 20:03:11 by lgomez-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

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
		time_action = time - philo->init_time;
		if (!philo->shared->someone_is_dead)
			printf("%ld %d %s\n", time_action, philo->nbr, action);
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
