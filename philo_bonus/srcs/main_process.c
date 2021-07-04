#include "../includes/philo.h"

int	throw_process(t_data *data)
{
	pid_t	pid;
	int	status;
	int i;

	i = 0;
	while (i < data->nbr_philos)
	{
		pid = fork();
		if (pid == -1)
		{
			printf("Error que hay que mejorar\n");
			exit(1);
		}
		else if (pid == 0)
			break;
		else
		{
			data->forks[i].pid = pid;
			i++;
		}
	}
	if (pid == 0)
	{
		data->nbr = i + 1;
		printf("soy el hijo %d\n", data->nbr);
		run_philo(data);
		exit(0);
	}
	else
	{
		wait(&status);
		i = 0;
		while (i < data->nbr_philos)
		{
			kill(data->forks[i].pid, SIGKILL);
			i++;
		}
		
		waitpid(pid, &status, 0);
	}
	return (0);
}


void	print_change(t_data *data, char *action, time_t time)
{
	time_t	time_action;

	
	sem_wait(data->sem_print);
	time_action = time - data->init_time;
	printf("%ld %d %s\n", time_action, data->nbr, action);
	sem_post(data->sem_print);

}

void	print_dead(t_data *data)
{
	time_t	time_action;
	sem_wait(data->sem_print);
	time_action = get_time() - data->init_time;
	printf("%ld %d died\n", time_action, data->nbr);
	exit (0);
}

