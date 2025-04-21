/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_sleep.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manufern <manufern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 14:36:07 by manufern          #+#    #+#             */
/*   Updated: 2025/04/21 15:05:24 by manufern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../filo.h"

void	ft_print_sleep(t_filo *philo, int philosopher_id)
{
	long	timestamp;

	pthread_mutex_lock(&philo->death_mutex);
	if (philo->philosopher_dead)
	{
		pthread_mutex_unlock(&philo->death_mutex);
		return ;
	}
	pthread_mutex_unlock(&philo->death_mutex);

	timestamp = get_current_time_ms() - philo->start_time;

	pthread_mutex_lock(&philo->print);
	printf("🛌 %ld %d is sleeping 🛌\n", timestamp, philosopher_id + 1);
	pthread_mutex_unlock(&philo->print);
}
