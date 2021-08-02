/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgomez-d <lgomez-d@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/05 19:44:11 by lgomez-d          #+#    #+#             */
/*   Updated: 2021/07/28 20:58:55 by lgomez-d         ###   ########.fr       */
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


static void	run_process(t_data *data)
{
	int i;

	if (sem_wait(data->sem_catch_forks) == -1)
		show_error("Sem_wait error");
	//printf("\x1b[34m%ld el proceso %d ha entrado a coger tenedores\x1b[37m\n", time_spent(data), data->nbr);
	if (data->nbr_philos == 1)
	{
		run_die(data);
	}
	i = 0;
	while (i < data->nbr_philos)
	{
		data->forks[i].pro = data->nbr;
		data->forks[i].needed = 1;
		if (pthread_create(&data->forks[i].id_thread, NULL, wait_sem, &data->forks[i]))
			show_error("pthread_create error");
		i++;
	}

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
	if (sem_post(data->sem_catch_forks) == -1)
		show_error("sem_post error");
	i = 0;
	while (i < data->nbr_philos)
	{
		if (data->forks[i].nbr != data->fork1->nbr && data->forks[i].nbr != data->fork2->nbr)
			data->forks[i].needed = 0;
		pthread_detach(data->forks[i].id_thread);		
		data->forks[i].id_thread = 0;
		i++;
	}
	
	i = 0;
	while (i < data->nbr_philos)
	{
		if (data->forks[i].nbr != data->fork1->nbr && data->forks[i].nbr != data->fork2->nbr)
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
