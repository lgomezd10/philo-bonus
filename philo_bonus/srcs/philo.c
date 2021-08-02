/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgomez-d <lgomez-d@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/05 19:44:11 by lgomez-d          #+#    #+#             */
/*   Updated: 2021/08/02 17:33:47 by lgomez-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

static void	*wait_sem(void *data)
{
	t_fork	*data_fork;

	data_fork = (t_fork *)data;
	if (data_fork->needed)
	{
		if (sem_wait(data_fork->sem) == -1)
			show_error("Sem_wait error");
		if (data_fork->needed)
		{
			data_fork->capture = 1;
		}
		else
		{
			if (sem_post(data_fork->sem) == -1)
				show_error("sem_post error");
		}
	}
	return (0);
}

static void	catch_forks(t_data *data)
{
	int	i;

	while (!data->fork1 || !data->fork2)
	{
		i = 0;
		while (i < data->nbr_philos)
		{
			if (data->forks[i].capture == 1)
			{
				if (!data->fork1)
				{
					data->fork1 = &data->forks[i];
					print_change(data, "has taken a fork");
				}
				else if (!data->fork2 && data->forks[i].nbr != data->fork1->nbr)
				{
					data->fork2 = &data->forks[i];
					print_change(data, "has taken a fork");
				}
			}
			if (time_spent(data) >= data->time_to_die)
				run_die(data);
			i++;
		}
	}
}

static void	free_forks(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nbr_philos)
	{
		if (!fork_in_use(data, data->forks[i]))
			data->forks[i].needed = 0;
		pthread_detach(data->forks[i].id_thread);
		data->forks[i].id_thread = 0;
		i++;
	}
	i = 0;
	while (i < data->nbr_philos)
	{
		if (!fork_in_use(data, data->forks[i]))
		{
			if (data->forks[i].capture)
			{
				if (sem_post(data->forks[i].sem) == -1)
					show_error("sem_post error");
				data->forks[i].capture = 0;
			}
		}
		i++;
	}
}

void	run_process(t_data *data)
{
	int		i;
	t_fork	*fork;

	if (sem_wait(data->sem_catch_forks) == -1)
		show_error("Sem_wait error");
	if (data->nbr_philos == 1)
		run_die(data);
	i = 0;
	while (i < data->nbr_philos)
	{
		fork = &data->forks[i];
		fork->pro = data->nbr;
		fork->needed = 1;
		if (pthread_create(&fork->id_thread, NULL, wait_sem, fork))
			show_error("pthread_create error");
		i++;
	}
	catch_forks(data);
	if (sem_post(data->sem_catch_forks) == -1)
		show_error("sem_post error");
	free_forks(data);
	run_eat(data);
	run_sleep(data);
}

void	run_philo(t_data *data)
{
	data->last_meal = get_time();
	while (data->times_must_eat == -1 || data->times_must_eat)
	{
		run_process(data);
		if (data->times_must_eat > 0)
			data->times_must_eat--;
	}
	exit(0);
}
