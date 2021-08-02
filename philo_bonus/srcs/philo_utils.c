/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgomez-d <lgomez-d@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/05 20:01:56 by lgomez-d          #+#    #+#             */
/*   Updated: 2021/08/02 16:32:18 by lgomez-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int	run_die(t_data *data)
{
	time_t	time_sleep;

	time_sleep = data->time_to_die - time_spent(data);
	if (time_sleep > 0)
		usleep(time_sleep * 1000);
	print_dead(data);
	return (1);
}

int	run_action(t_data *data, useconds_t time_action)
{
	time_t	finish;
	time_t	time;

	finish = get_time() + time_action;
	if (time_spent(data) + time_action >= data->time_to_die)
		run_die(data);
	if (time_action > 20)
		usleep((time_action - 10) * (useconds_t)1000);
	time = get_time();
	while (time < finish)
		time = get_time();
	return (0);
}

int	run_sleep(t_data *data)
{
	time_t	next_meal;

	print_change(data, "is sleeping");
	if (run_action(data, data->time_to_sleep))
		return (1);
	print_change(data, "is thinking");
	next_meal = data->time_to_eat - data->time_to_sleep;
	if (next_meal >= 0 && \
		time_spent(data) + next_meal > data->time_to_die)
		return (run_die(data));
	return (0);
}

int	run_eat(t_data *data)
{
	if (time_spent(data) >= data->time_to_die)
		return (run_die(data));
	data->last_meal = get_time();
	print_change(data, "\x1b[31mis eating");
	run_action(data, data->time_to_eat);
	if (sem_post(data->fork1->sem) == -1)
		show_error("sem_post error");
	if (sem_post(data->fork2->sem) == -1)
		show_error("sem_post error");
	data->fork1->capture = 0;
	data->fork2->capture = 0;
	data->fork1 = 0;
	data->fork2 = 0;
	return (0);
}

int	fork_in_use(t_data *data, t_fork fork)
{
	int	in_use;

	in_use = fork.nbr == data->fork1->nbr;
	in_use = in_use || fork.nbr == data->fork2->nbr;
	return (in_use);
}
