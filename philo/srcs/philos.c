/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philos.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgomez-d <lgomez-d@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/30 18:46:13 by lgomez-d          #+#    #+#             */
/*   Updated: 2021/07/27 20:56:49 by lgomez-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

static int	run_die(t_philo *philo)
{
	time_t	time_sleep;

	time_sleep = philo->shared->time_to_die - time_spent(philo);
	if (time_sleep > 0)
		usleep(time_sleep * 1000);
	print_dead(philo);
	return (1);
}

static int	run_action(t_philo *philo, useconds_t time_action)
{
	time_t	finish;

	finish = get_time() + time_action;
	if (time_spent(philo) + time_action >= philo->shared->time_to_die)
		run_die(philo);
	if (time_action > 10)
		usleep((time_action - 10) * (useconds_t)1000);
	while (get_time() < finish)
		usleep(1000);
	return (0);
}

static int	run_sleep(t_philo *philo)
{
	print_change(philo, "is sleeping", get_time());
	if (run_action(philo, philo->shared->time_to_sleep))
		return (1);
	print_change(philo, "is thinking", get_time());
	if (philo->shared->nbr_philos % 2 != 0 && philo->nbr == 1)
		usleep(2000);
	return (0);
}

static int	run_eat(t_philo *philo)
{
	if (time_spent(philo) >= philo->shared->time_to_die)
		return (run_die(philo));
	pthread_mutex_lock(philo->mutex1);
	if (!philo->shared->someone_is_dead)
	{
		print_change(philo, "has taken a fork 1", get_time());
		if (philo->shared->nbr_philos == 1)
			return (run_die(philo));
		if (philo->nbr == philo->shared->nbr_philos && philo->nbr % 2 != 0)
			usleep(1000);
		pthread_mutex_lock(philo->mutex2);
		if (!philo->shared->someone_is_dead)
		{
			print_change(philo, "has taken a fork 2", get_time());
			if (time_spent(philo) < philo->shared->time_to_die)
			{
				philo->last_meal = get_time();
				print_change(philo, "\x1b[31mis eating", get_time());
			}
			run_action(philo, philo->shared->time_to_eat);
		}
		pthread_mutex_unlock(philo->mutex1);
	}
	pthread_mutex_unlock(philo->mutex2);
	return (0);
}

void	*run_thread(void *data_philo)
{
	t_philo	*philo;

	philo = (t_philo *)data_philo;
	philo->init_time = get_time();
	philo->last_meal = get_time();
	philo->mutex1 = &philo->fork_right->mutex;
	philo->mutex2 = &philo->fork_left->mutex;
	if (philo->nbr % 2 != 0)
	{
		printf("soy el impar %d y mi primer tenedor es el %d\n", philo->nbr, philo->fork_left->nbr);
		philo->mutex1 = &philo->fork_left->mutex;
		philo->mutex2 = &philo->fork_right->mutex;
	}
	else
		printf("soy el par %d y mi primer tenedor es el %d\n", philo->nbr, philo->fork_right->nbr);
	if (philo->nbr % 2 == 0 || philo->nbr == philo->shared->nbr_philos)
		usleep(2000);
	while (!philo->shared->someone_is_dead && \
		(philo->times_must_eat < 0 || philo->times_must_eat))
	{
		if (philo->nbr == philo->shared->nbr_philos && philo->nbr % 2 != 0)
			usleep(1000);
		run_eat(philo);
		if (!philo->shared->someone_is_dead)
			run_sleep(philo);
		philo->times_must_eat--;
	}
	philo->end = 1;
	return (0);
}
