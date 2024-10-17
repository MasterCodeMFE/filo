/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   think.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manufern <manufern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 12:21:55 by manufern          #+#    #+#             */
/*   Updated: 2024/10/03 14:30:33 by manufern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../filo.h"

void	ft_think(t_filo *philo, int philosopher_id)
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
	pthread_mutex_lock(&data->print);
	pthread_mutex_lock(&data->death_mutex);
	if (data->philosopher_dead)
	{
		pthread_mutex_unlock(&data->death_mutex);
		pthread_mutex_unlock(&data->print);
		return ;
	}
	pthread_mutex_unlock(&data->death_mutex);
	printf("🧑 %ld %d is thinking 🧑\n",
		get_current_time_ms() - data->start_time, philosopher_id + 1);
	pthread_mutex_unlock(&data->print);
}
