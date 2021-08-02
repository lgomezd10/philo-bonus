/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgomez-d <lgomez-d@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/30 20:22:16 by lgomez-d          #+#    #+#             */
/*   Updated: 2021/08/02 15:33:45 by lgomez-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int	ft_atol(const char *nptr)
{
	long	nbr;
	int		i;

	nbr = 0;
	i = 0;
	while ((nptr[i] >= '\t' && nptr[i] <= '\r') || nptr[i] == ' ')
		i++;
	while (nptr[i] && (nptr[i] >= '0' && nptr[i] <= '9'))
	{
		nbr *= 10;
		nbr += nptr[i] - '0';
		if (nbr > 2147483647)
			return (-1);
		i++;
	}
	if (nptr[i] && (nptr[i] < '0' || nptr[i] > '9'))
		return (-1);
	else
		return ((int)(nbr));
}

time_t	get_time(void)
{
	struct timeval	act_time;
	time_t			time;

	if (gettimeofday(&act_time, NULL) == -1)
		return (-1);
	time = act_time.tv_sec * 1000 + act_time.tv_usec / 1000;
	return (time);
}

time_t	time_spent(t_philo *philo)
{
	return (get_time() - philo->last_meal);
}

void	clean_all(t_data *data)
{
	int	i;

	i = 0;
	if (data->forks)
	{
		while (i < data->shared.nbr_philos)
		{
			pthread_mutex_destroy(&data->forks[i].mutex);
			i++;
		}
	}
	pthread_mutex_destroy(&data->shared.mutex_print);
	free(data->philos);
	free(data->forks);
}

int	nbr_philos_odd(t_philo *philo)
{
	return (philo->shared->nbr_philos % 2 != 0);
}
