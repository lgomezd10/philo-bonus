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
	data->last_meal = get_time();
	print_change(data, "is eating", get_time());
	run_action(data, data->time_to_eat);
	if(sem_post(data->fork1->sem) == - 1)
		show_error("Not work semaphore");
	if(sem_post(data->fork2->sem) == - 1)
		show_error("Not work semaphore");
	data->fork1->capture = 0;
	data->fork2->capture = 0;
	data->fork1 = 0;
	data->fork2 = 0;
	return (0);
}