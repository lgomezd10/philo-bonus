/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgomez-d <lgomez-d@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/28 20:30:21 by lgomez-d          #+#    #+#             */
/*   Updated: 2021/07/28 20:36:30 by lgomez-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	do_mutex_catch_fork(t_philo *philo)
{
	int	pos;

	pos = philo->shared->pos;
	if (nbr_philos_odd(philo))
	{
		while (pos != get_pos(philo) - 1 && pos != get_pos(philo))
			pos = philo->shared->pos;
		if (philo->nbr == philo->shared->nbr_philos)
			usleep(1000);
	}
	pthread_mutex_lock(&philo->shared->catch_fork);
	philo->shared->pos = get_pos(philo);
	if (philo->nbr == 1)
		philo->shared->pos = 1;
}

int	get_pos(t_philo *philo)
{
	if (!nbr_philos_odd(philo))
		return (0);
	if (philo->shared->nbr_philos > 3)
	{
		if (philo->nbr == 1)
			return (5);
		if (philo->nbr % 2 != 0 && philo->nbr != philo->shared->nbr_philos)
			return (2);
		if (philo->nbr % 2 == 0)
			return (3);
		return (4);
	}
	else
	{
		if (philo->nbr == 1)
			return (4);
		if (philo->nbr % 2 == 0)
			return (2);
		return (3);
	}
}
