/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_sleep.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manufern <manufern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 14:36:07 by manufern          #+#    #+#             */
/*   Updated: 2024/10/03 14:40:19 by manufern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../filo.h"

void	ft_print_sleep(t_filo *philo, int philosopher_id)
{
	t_filo	*data;

	data = (t_filo *)philo;
	pthread_mutex_lock(&data->print);
	pthread_mutex_lock(&data->death_mutex);
	if (data->philosopher_dead)
	{
		pthread_mutex_unlock(&data->print);
		pthread_mutex_unlock(&data->death_mutex);
		return ;
	}
	pthread_mutex_unlock(&data->death_mutex);
	printf("🛌 %ld %d is sleeping 🛌\n",
		get_current_time_ms() - data->start_time, philosopher_id + 1);
	pthread_mutex_unlock(&data->print);
}
