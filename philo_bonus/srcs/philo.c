/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgomez-d <lgomez-d@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/05 19:44:11 by lgomez-d          #+#    #+#             */
/*   Updated: 2021/07/08 20:47:27 by lgomez-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

static void	*wait_sem(void *data)
{
	t_fork	*fork;

	fork = (t_fork *)data;
	if (sem_wait(fork->sem) == -1)
		show_error("Sem_wait error");
	fork->capture = 1;
	return (0);
}

static t_fork	*get_second_fork(t_data *data, int nbr)
{
	int	half;

	half = data->nbr_philos / 2;
	if (data->nbr_philos == 1)
		return (0);
	if (data->nbr_philos % 2 != 0 && nbr == data->nbr_philos)
		return (&data->forks[0]);
	else if (nbr > half)
		return (&data->forks[(nbr - half) - 1]);
	else
		return (&data->forks[(nbr + half) - 1]);
}

static void	run_process(t_data *data)
{
	data->fork1 = &data->forks[data->nbr - 1];
	if (pthread_create(&data->fork1->id_thread, NULL, wait_sem, data->fork1))
		show_error("pthread_create error");
	data->fork2 = get_second_fork(data, data->nbr);
	if (data->fork2 && \
		pthread_create(&data->fork2->id_thread, NULL, wait_sem, data->fork2))
		show_error("pthread_create error");
	while (data->fork2 && !data->fork1->capture && !data->fork2->capture)
	{
		usleep(1000);
		if (time_spent(data) > data->time_to_die)
			run_die(data);
	}
	print_change(data, "has taken a fork");
	if (data->nbr_philos == 1)
		run_die(data);
	while (!data->fork1->capture || !data->fork2->capture)
	{
		usleep(1000);
		if (time_spent(data) > data->time_to_die)
			run_die(data);
	}
	print_change(data, "has taken a fork");
	run_eat(data);
	run_sleep(data);
}

void	run_philo(t_data *data)
{
	data->last_meal = get_time();
	if (data->nbr > data->nbr_philos / 2)
		usleep(2000);
	while (data->times_must_eat == -1 || data->times_must_eat)
	{
		if (data->nbr > data->nbr_philos / 2)
		usleep(2000);
		else if (data->nbr == data->nbr_philos && data->nbr % 2 != 0)
			usleep(1000);
		run_process(data);
		if (data->times_must_eat > 0)
			data->times_must_eat--;
	}
	exit(0);
}
