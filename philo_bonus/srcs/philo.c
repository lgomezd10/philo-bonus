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
	print_change(data, "has taken a fork", get_time());
	run_action(data, data->time_to_eat);
	free_all_forks(data);
	return (0);
}

void *wait_sem(void *data)
{
	t_fork *fork;

	fork = (t_fork *)data;
	if (sem_wait(fork->sem) == -1)
		show_error("Not work semaphore");
	print_change(data, "is eating", get_time());
	fork->capture = 1;
	return (0);
}

int catch_forks(t_data *data)
{
	int captured;
	int i;
	int firts_try;
	time_t time;

	captured = 0;
	firts_try = 1;
	time = get_time();
	while (captured < 2)
	{
		i = 0;
		while (captured < 2 && i < data->nbr_philos)
		{
			if (data->forks[i].capture)
				captured = check_captured_fork(data, i);
			i++;          
		}
		if (firts_try && captured == 1 && get_time() - time >= 5 && get_time() - time < 100)
		{
			printf("EL PHILO %d  libera el tenedor despues de que %ld - %ld sea %ld\n", data->nbr, get_time(), time, get_time() - time);
			free_all_forks(data);
			usleep(10000);
			firts_try = 0;
		}
	}
	return (0);
}

t_fork *get_second_fork(t_data *data, int nbr)
{
	int half;

	half = data->nbr_philos / 2;
	if (nbr > half)
		return (&data->forks[(nbr - half) - 1]);
	else
		run_eat (&data->forks[(nbr + half) - 1]);
}

int run_process(t_data *data)
{
	int has_error;
	
	// si es la primera vez que entra y soy par, si no consigo el segundo tenedor en x tiempo. Soltamos todo y dormimos x tiempo
	data->fork1 = &data->forks[data->nbr - 1];
	has_error = pthread_create(data->fork1, NULL, wait_sem, data->fork1);
	if (has_error)
		return (1);
	data->fork2 = get_second_fork(data, data->nbr);
	has_error = pthread_create(data->fork2, NULL, wait_sem, data->fork2);
	if (has_error)
		return (1);
	pthread_join(data->fork1->id_thread, NULL);
	pthread_join(data->fork2->id_thread, NULL);
	run_eat(data);
	run_sleep(data);
	return (0);
}

int run_philo(t_data *data)
{
	data->init_time = get_time();
	data->last_meal = get_time();
	if (data->nbr > data->nbr_philos / 2)
	{
		printf("el hijo %d va a dormir\n", data->nbr);
		usleep(2000);
	}
	while (1)
	{
		
		run_process(data);
	}
	return (0);
}