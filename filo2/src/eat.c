/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eat.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manufern <manufern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 12:23:17 by manufern          #+#    #+#             */
/*   Updated: 2024/10/03 13:00:40 by manufern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../filo.h"

void	ft_eat(t_filo *philo, int philosopher_id, int left_fork, int right_fork)
{
	t_filo	*data;

	data = (t_filo *)philo;
	pthread_mutex_lock(&data->death_mutex);
	if (data->philosopher_dead)
	{
		pthread_mutex_unlock(&data->death_mutex);
		return ;
	}
	pthread_mutex_unlock(&data->death_mutex);
	pthread_mutex_lock(&data->meal_time_mutex);
	data->last_meal_time[philosopher_id] = get_current_time_ms();
	pthread_mutex_unlock(&data->meal_time_mutex);
	pthread_mutex_lock(&data->print);
	printf("🍽️  %ld %d is eating 🍽️\n",
		data->last_meal_time[philosopher_id] - data->start_time,
		philosopher_id + 1);
	pthread_mutex_unlock(&data->print);
	my_usleep(data->time_to_eat * 1000);
	pthread_mutex_unlock(&data->forks[left_fork]);
	pthread_mutex_unlock(&data->forks[right_fork]);
	data->eat[philosopher_id] = 1;
	if (data->number_of_eats[philosopher_id] != -1)
		data->number_of_eats[philosopher_id]++;
}
