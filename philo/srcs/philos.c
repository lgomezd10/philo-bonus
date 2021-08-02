/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philos.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgomez-d <lgomez-d@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/30 18:46:13 by lgomez-d          #+#    #+#             */
/*   Updated: 2021/07/28 20:27:37 by lgomez-d         ###   ########.fr       */
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
	//time_t	finish;
	//time_t	time;

	//finish = get_time() + time_action;
	if (time_spent(philo) + time_action >= philo->shared->time_to_die)
		run_die(philo);
		/*
	if (time_action > 20)
		usleep((time_action - 10) * (useconds_t)1000);
		*/
	/*
	time = get_time();
	while (time <= finish)
		time = get_time();
		*/
	usleep(time_action * 1000);
	return (0);
}

static int	run_sleep(t_philo *philo)
{
	int time_wait;
	print_change(philo, "is sleeping");
	if (run_action(philo, philo->shared->time_to_sleep))
		return (1);
	print_change(philo, "is thinking");
	if (nbr_philos_odd(philo))
	{
		printf("No debería entrar aquí\n");
		time_wait = philo->shared->time_to_eat * 3;
		while (time_spent(philo) < time_wait)
			usleep(100);
	}	
	return (0);
}

static int	run_eat(t_philo *philo)
{
	if (time_spent(philo) >= philo->shared->time_to_die)
		return (run_die(philo));
	pthread_mutex_lock(&philo->fork1->mutex);
	if (!philo->shared->someone_is_dead)
	{
		print_change(philo, "has taken a fork");
		if (philo->shared->nbr_philos == 1)
			return (run_die(philo));
		pthread_mutex_lock(&philo->fork2->mutex);
		//pthread_mutex_unlock(&philo->shared->catch_fork);
		if (!philo->shared->someone_is_dead)
		{
			print_change(philo, "has taken a fork");
			if (time_spent(philo) < philo->shared->time_to_die)
			{
				philo->last_meal = get_time();
				print_change(philo, "\x1b[31mis eating");
			}
			run_action(philo, philo->shared->time_to_eat);
		}
		pthread_mutex_unlock(&philo->fork2->mutex);
	}
	pthread_mutex_unlock(&philo->fork1->mutex);
	return (0);
}

void	*run_thread(void *data_philo)
{
	t_philo	*philo;

	philo = (t_philo *)data_philo;
	philo->init_time = get_time();
	philo->last_meal = get_time();
	philo->fork1 = philo->fork_right;
	philo->fork2 = philo->fork_left;
	if (philo->nbr % 2 == 0)
	{
		philo->fork1 = philo->fork_left;
		philo->fork2 = philo->fork_right;
	}
	if (philo->nbr % 2 == 0 || philo->nbr == philo->shared->nbr_philos)
		usleep(5000);
	while (!philo->shared->someone_is_dead && \
		(philo->times_must_eat < 0 || philo->times_must_eat))
	{
		//pthread_mutex_lock(&philo->shared->catch_fork);
		run_eat(philo);
		if (!philo->shared->someone_is_dead)
			run_sleep(philo);
		philo->times_must_eat--;
	}
	philo->end = 1;
	return (0);
}
