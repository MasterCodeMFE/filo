/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manufern <manufern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 12:21:22 by manufern          #+#    #+#             */
/*   Updated: 2024/10/03 13:14:30 by manufern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../filo.h"

void	ft_dead(t_filo *philo, int i, long current_time)
{
	t_filo	*data;

	data = (t_filo *)philo;
	pthread_mutex_lock(&data->death_mutex);
	data->philosopher_dead = 1;
	pthread_mutex_unlock(&data->death_mutex);
	pthread_mutex_unlock(&data->meal_time_mutex);
	pthread_mutex_lock(&data->print);
	printf("💀 %ld %d has died 💀\n", current_time - data->start_time, i + 1);
	pthread_mutex_unlock(&data->print);
}

void	*monitoring_aux(t_filo *philo, int i,
	int *finished_philosophers, pthread_mutex_t *finished_mutex)
{
	t_filo	*data;
	long	current_time;

	data = (t_filo *)philo;
	while (++i < data->number_philo)
	{
		if (data->number_of_times_eat != -1
			&& data->number_of_eats[i] >= data->number_of_times_eat)
		{
			pthread_mutex_lock(finished_mutex);
			(*finished_philosophers)++;
			pthread_mutex_unlock(finished_mutex);
			continue ;
		}
		current_time = get_current_time_ms();
		pthread_mutex_lock(&data->meal_time_mutex);
		if (current_time - data->last_meal_time[i] > data->time_to_die)
		{
			ft_dead(data, i, current_time);
			return (pthread_mutex_unlock(&data->meal_time_mutex), NULL);
		}
		pthread_mutex_unlock(&data->meal_time_mutex);
	}
	return (NULL);
}

void	*monitoring(void *philo)
{
	t_filo			*data;
	int				i;
	int				finished_philosophers;
	pthread_mutex_t	finished_mutex;

	data = (t_filo *)philo;
	finished_philosophers = 0;
	while (1)
	{
		i = -1;
		monitoring_aux(data, i, &finished_philosophers, &finished_mutex);
		pthread_mutex_lock(&finished_mutex);
		if (finished_philosophers >= data->number_philo)
		{
			pthread_mutex_unlock(&finished_mutex);
			break ;
		}
		pthread_mutex_unlock(&finished_mutex);
		pthread_mutex_lock(&data->death_mutex);
		if (data->philosopher_dead)
			return (pthread_mutex_unlock(&data->death_mutex), NULL);
		pthread_mutex_unlock(&data->death_mutex);
		usleep(100);
	}
	return (NULL);
}
