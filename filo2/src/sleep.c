/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sleep.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manufern <manufern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 12:22:21 by manufern          #+#    #+#             */
/*   Updated: 2024/10/03 14:39:36 by manufern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../filo.h"

void	ft_sleep(t_filo *philo, int philosopher_id)
{
	t_filo	*data;

	data = (t_filo *)philo;
	if (data->eat[philosopher_id] == 1)
	{
		pthread_mutex_lock(&data->death_mutex);
		if (data->philosopher_dead)
		{
			pthread_mutex_unlock(&data->death_mutex);
			return ;
		}
		pthread_mutex_unlock(&data->death_mutex);
		ft_print_sleep(data, philosopher_id);
		pthread_mutex_lock(&data->death_mutex);
		if (data->philosopher_dead)
		{
			pthread_mutex_unlock(&data->death_mutex);
			return ;
		}
		pthread_mutex_unlock(&data->death_mutex);
		my_usleep(data->time_to_sleep * 1000);
	}
}
