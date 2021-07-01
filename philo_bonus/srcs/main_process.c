#include "../includes/philo.h"

int	throw_process(t_data *data)
{
	int	pid;
	int	status;
	int i;

	i = 1;
	while (i <= data->nbr_philos)
	{
		pid = fork();
		if (pid == -1)
			printf("Error que hay que mejorar\n");
		else if (pid == 0)
		{
			data->nbr = i;
			printf("soy el hijo %d\n");
			exit(0);
		}
		i++;
	}
	waitpid(pid, &status, NULL);	
}

/*
void	print_change(t_philo *philo, char *action, time_t time)
{
	time_t	time_action;

	if (!philo->shared->someone_is_dead)
	{
		pthread_mutex_lock(&philo->shared->mutex_print);
		time_action = time - philo->init_time;
		if (!philo->shared->someone_is_dead)
			printf("%ld %d %s\n", time_action, philo->nbr, action);
		pthread_mutex_unlock(&philo->shared->mutex_print);
	}
}

void	print_dead(t_philo *philo)
{
	time_t	time_action;

	if (!philo->shared->someone_is_dead)
	{
		pthread_mutex_lock(&philo->shared->mutex_print);
		if (!philo->shared->someone_is_dead)
		{
			time_action = get_time() - philo->init_time;
			printf("%ld %d died\n", time_action, philo->nbr);
			philo->shared->someone_is_dead = 1;
		}
		pthread_mutex_unlock(&philo->shared->mutex_print);
	}
}
*/
