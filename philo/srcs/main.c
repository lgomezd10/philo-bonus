/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgomez-d <lgomez-d@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/30 20:03:09 by lgomez-d          #+#    #+#             */
/*   Updated: 2021/06/30 20:03:33 by lgomez-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int	main(int argc, char **argv)
{
	t_data	data;

	memset(&data, '\0', sizeof(t_data));
	if (argc < 5 || argc > 6)
	{
		printf("Needed four or five arguments\n");
		return (1);
	}
	if (!load_arguments(&data, argc, argv))
	{
		printf("Argements must be positive int numbers\n");
		return (1);
	}
	if (load_forks(&data) || load_philos(&data))
	{
		printf("System error\n");
		return (1);
	}
	return (throw_threads(&data));
}
