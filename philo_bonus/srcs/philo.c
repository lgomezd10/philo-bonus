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

	finish = get_time() + time_action;
	if (time_spent(data) + time_action >= data->time_to_die)
		run_die(data);
	usleep((time_action - 5) * (useconds_t)1000);
	while (get_time() < finish)
		usleep(1000);
	return (0);
}

int	run_sleep(t_data *data)
{
	time_t	next_meal;

	print_change(data, "is sleeping", get_time());
	if (run_action(data, data->time_to_sleep))
		return (1);
	print_change(data, "is thinking", get_time());
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
	print_change(data, "has taken a fork", get_time());
	data->last_meal = get_time();
	print_change(data, "is eating", get_time());	
	run_action(data, data->time_to_eat);
	sem_post(data->fork1->sem);
	sem_post(data->fork2->sem);
	data->fork1 = 0;
	data->fork2 = 0;
	return (0);
}

void *wait_sem(void *data)
{
	t_fork *fork;

	fork = (t_fork *)data;
	if (sem_wait(data) == -1)
		show_error("Not work semaphore");
	fork->capture = 1;
	while (1)
		usleep(1000);
	return (0);
}

int run_process(t_data *data)
{
	int i;
	int has_error;
	int has_two;
	t_fork *fork;

	i = 0;	
	data->init_time = get_time();
	data->last_meal = get_time();
	while (i < data->nbr_philos)
	{
		fork = &data->forks[i];
		has_error = pthread_create(&fork->id_thread, NULL, wait_sem, fork);
		if (has_error)
			return (1);
		i++;
	}
	has_two = 0;
	while (!has_two)
	{
		i = 0;
		while (!has_two && i < data->nbr_philos)
		{
			if (data->forks[i].capture)
			{
				if (!data->fork1)
				{
					data->fork1 = &data->forks[i];
					print_change(data, "has taken a fork", get_time());
					if (data->nbr_philos == 1)
						return (run_die(data));
				}
				else if (!data->fork2 && data->forks[i].nbr != data->fork1->nbr)
				{
					data->fork2 = &data->forks[i];
					print_change(data, "has taken a fork", get_time());
				}
				else if (data->fork1 && data->fork2)
					has_two = 1;
			}
			i++;          
		}      
	}
	i = 0;
	while (i < data->nbr_philos)
	{
		pthread_detach(data->forks[i].id_thread);
		i++;
	}
	i = 0;
	has_two = 0;
	while (i < data->nbr_philos)
	{
		if (data->forks[i].capture)
		{
			if (data->forks[i].nbr != data->fork1->nbr && data->forks[i].nbr != data->fork2->nbr)
			{
				if(sem_post(data->forks[i].sem) == - 1)
					show_error("Not work semaphore");
			}
		}
		i++;
	}
	run_eat(data);
	return (0);
}

int run_philo(t_data *data)
{
	while (1)
	{
		run_process(data);
	}
	return (0);
}