/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_process.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgomez-d <lgomez-d@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/05 19:49:35 by lgomez-d          #+#    #+#             */
/*   Updated: 2021/07/08 20:48:33 by lgomez-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

static void	wait_process(t_data *data)
{
	int	status;
	int	i;
	int	nbr_philos;

	status = 0;
	nbr_philos = data->nbr_philos;
	while (!status && nbr_philos)
	{
		waitpid(-1, &status, 0);
		nbr_philos--;
	}
	i = 0;
	while (i < data->nbr_philos)
	{
		kill(data->forks[i].pid, SIGKILL);
		i++;
	}
	i = 0;
	while (i < data->nbr_philos)
	{
		sem_close(data->forks[i].sem);
		sem_unlink(data->forks[i].name);
		i++;
	}
	sem_unlink(SEM_PRINT);
	sem_close(data->sem_print);
	clean_all(data);
	exit (0);
}

void	throw_process(t_data *data)
{
	pid_t	pid;
	int		i;

	i = 0;
	data->init_time = get_time();
	while (i < data->nbr_philos)
	{
		pid = fork();
		if (pid == -1)
			show_error("fork error");
		else if (pid == 0)
			break ;
		else
		{
			data->forks[i].pid = pid;
			i++;
		}
	}
	if (pid == 0)
	{
		data->nbr = i + 1;
		run_philo(data);
	}
	else
	{
		wait_process(data);
	}
}

void	print_change(t_data *data, char *action)
{
	time_t	time_action;

	if (sem_wait(data->sem_print) == -1)
		show_error("Error in semaphore print change");
	time_action = get_time() - data->init_time;
	printf("%ld %d %s\n", time_action, data->nbr, action);
	sem_post(data->sem_print);
}

void	print_dead(t_data *data)
{
	time_t	time_action;

	if (sem_wait(data->sem_print) == -1)
		show_error("Error in semaphore print dead");
	time_action = get_time() - data->init_time;
	printf("%ld %d died\n", time_action, data->nbr);
	exit (1);
}
