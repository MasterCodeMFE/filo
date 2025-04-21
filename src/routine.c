/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manufern <manufern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 12:25:45 by manufern          #+#    #+#             */
/*   Updated: 2025/04/21 15:07:06 by manufern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../filo.h"

int	is_philosopher_dead(t_filo *philo)
{
	int	dead;

	pthread_mutex_lock(&philo->death_mutex);
	dead = philo->philosopher_dead;
	pthread_mutex_unlock(&philo->death_mutex);
	return (dead);
}

void	*ft_action(t_filo *philo, int id, int left, int right)
{
	if (is_philosopher_dead(philo))
		return (NULL);

	philo->eat[id] = 0;

	if (is_philosopher_dead(philo))
		return (NULL);

	ft_take_fork(philo, id, left, right);

	if (is_philosopher_dead(philo))
		return (NULL);

	ft_sleep(philo, id);

	if (is_philosopher_dead(philo))
		return (NULL);

	ft_think(philo, id);

	return (NULL);
}

void	*routine(void *arg)
{
	t_filo	*data = (t_filo *)arg;
	int		id;
	int		left;
	int		right;

	pthread_mutex_lock(&data->id_mutex);
	id = data->philosopher_id++;
	pthread_mutex_unlock(&data->id_mutex);

	left = id;
	right = (id + 1) % data->number_philo;

	if (data->number_philo < 2)
		return (NULL);

	if ((id + 1) % 2 != 0)
		my_usleep(500);

	while (!is_philosopher_dead(data)
		&& (data->number_of_times_eat == -1
			|| data->number_of_eats[id] < data->number_of_times_eat))
	{
		ft_action(data, id, left, right);
	}
	return (NULL);
}
