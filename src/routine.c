/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manufern <manufern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 12:25:45 by manufern          #+#    #+#             */
/*   Updated: 2024/10/03 15:39:04 by manufern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../filo.h"

void	*ft_action(t_filo *philo, int philosopher_id,
	int left_fork, int right_fork)
{
	t_filo	*data;

	data = (t_filo *)philo;
	pthread_mutex_lock(&data->death_mutex);
	if (data->philosopher_dead)
		return (pthread_mutex_unlock(&data->death_mutex), NULL);
	pthread_mutex_unlock(&data->death_mutex);
	data->eat[philosopher_id] = 0;
	pthread_mutex_lock(&data->death_mutex);
	if (data->philosopher_dead)
		return (pthread_mutex_unlock(&data->death_mutex), NULL);
	pthread_mutex_unlock(&data->death_mutex);
	ft_take_fork(data, philosopher_id, left_fork, right_fork);
	ft_sleep(data, philosopher_id);
	pthread_mutex_lock(&data->death_mutex);
	if (data->philosopher_dead)
		return (pthread_mutex_unlock(&data->death_mutex), NULL);
	pthread_mutex_unlock(&data->death_mutex);
	ft_think(data, philosopher_id);
	return (NULL);
}

void	*routine(void *philo)
{
	t_filo	*data;
	int		philosopher_id;
	int		left_fork;
	int		right_fork;

	data = (t_filo *)philo;
	pthread_mutex_lock(&data->id_mutex);
	philosopher_id = data->philosopher_id++;
	pthread_mutex_unlock(&data->id_mutex);
	left_fork = philosopher_id;
	right_fork = (philosopher_id + 1) % data->number_philo;
	if (data->number_philo < 2)
		return (NULL);
	if ((philosopher_id + 1) % 2 != 0)
		my_usleep(500);
	while (data->philosopher_dead == 0
		&& (data->number_of_eats[philosopher_id] < data->number_of_times_eat
			|| data->number_of_times_eat == -1))
		ft_action(data, philosopher_id, left_fork, right_fork);
	return (NULL);
}
