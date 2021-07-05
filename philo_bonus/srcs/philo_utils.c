#include "../includes/philo.h"

void free_forks_needless(t_data *data)
{
	int i;
	t_fork *fork;

	i = 0;
	while (i < data->nbr_philos)
	{
		if (data->forks[i].capture)
		{
			fork = &data->forks[i];
			if (fork->nbr != data->fork1->nbr && fork->nbr != data->fork2->nbr)
			{
				if(sem_post(fork->sem) == - 1)
					show_error("Not work semaphore");
				data->forks[i].capture = 0;
			}
		}
		i++;
	}
}

void free_all_forks(t_data *data)
{
	int i;

	i = 0;
	while (i < data->nbr_philos)
	{
		if (data->forks[i].capture)
		{
			if(sem_post(data->forks[i].sem) == - 1)
				show_error("Not work semaphore");
			data->forks[i].capture = 0;			
		}
		i++;
	}
	data->fork1 = 0;
	data->fork2 = 0;
}

int check_captured_fork(t_data *data, int nbr)
{
	int captured;

	captured = 0;
	if (!data->fork1)
	{
		data->fork1 = &data->forks[nbr];
		printf("el philo %d has taken el primer fork: %d\n", data->nbr, data->fork1->nbr);
		print_change(data, "has taken a fork 1", get_time());
		if (data->nbr_philos == 1)
			return (run_die(data));
		captured = 1;
	}
	else if (!data->fork2 && data->forks[nbr].nbr != data->fork1->nbr)
	{
		data->fork2 = &data->forks[nbr];
		printf("el philo %d has taken el segundo fork: %d\n", data->nbr, data->fork2->nbr);
		print_change(data, "has taken a fork 2", get_time());
		captured = 2;
	}
	return (captured);
}