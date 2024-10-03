/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_mutex.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manufern <manufern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 12:29:14 by manufern          #+#    #+#             */
/*   Updated: 2024/10/03 13:07:13 by manufern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../filo.h"

t_filo	*init_mutex(t_filo *philo)
{
	int	i;

	i = -1;
	philo->forks = malloc(sizeof(pthread_mutex_t) * philo->number_philo);
	if (philo->forks == NULL)
		return (printf("Error: Memory allocation failed\n"), NULL);
	while (++i < philo->number_philo)
	{
		if (pthread_mutex_init(&philo->forks[i], NULL) != 0)
		{
			printf("Error: Mutex initialization failed\n");
			free(philo->forks);
			return (NULL);
		}
	}
	if (pthread_mutex_init(&philo->print, NULL) != 0
		|| pthread_mutex_init(&philo->id_mutex, NULL) != 0
		|| pthread_mutex_init(&philo->meal_time_mutex, NULL) != 0
		|| pthread_mutex_init(&philo->death_mutex, NULL) != 0)
	{
		printf("Error: Mutex initialization failed\n");
		return (free(philo->forks), NULL);
	}
	philo->philosopher_dead = 0;
	return (philo);
}
