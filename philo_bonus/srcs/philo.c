#include "../includes/philo.h"

static void *wait_sem(void *data)
{
	t_fork *fork;

	fork = (t_fork *)data;
	if (sem_wait(fork->sem) == -1)
		show_error("Sem_wait error");	
	fork->capture = 1;
	return (0);
}

static t_fork *get_second_fork(t_data *data, int nbr)
{
	int half;

	half = data->nbr_philos / 2;
	if (data->nbr_philos % 2 != 0 && nbr == data->nbr_philos)
		return (&data->forks[0]);
	else if (nbr > half)
		return (&data->forks[(nbr - half) - 1]);
	else
		return (&data->forks[(nbr + half) - 1]);
}

static int run_process(t_data *data)
{
	data->fork1 = &data->forks[data->nbr - 1];
	if (pthread_create(&data->fork1->id_thread, NULL, wait_sem, data->fork1))
		show_error("pthread_create error");
	data->fork2 = get_second_fork(data, data->nbr);
	if (pthread_create(&data->fork2->id_thread, NULL, wait_sem, data->fork2))
		show_error("pthread_create error");
	while (!data->fork1->capture && !data->fork2->capture)
		usleep(1000);
	print_change(data, "has taken a fork", get_time());
	while (!data->fork1->capture || !data->fork2->capture)
		usleep(1000);
	print_change(data, "has taken a fork", get_time());
	run_eat(data);
	run_sleep(data);
	return (0);
}

int run_philo(t_data *data)
{
	data->init_time = get_time();
	data->last_meal = get_time();
	if (data->nbr > data->nbr_philos / 2)
		usleep(2000);
	while (data->times_must_eat == -1 || data->times_must_eat)
	{
		run_process(data);
		if (data->times_must_eat > 0)
			data->times_must_eat--;
	}
	return (0);
}