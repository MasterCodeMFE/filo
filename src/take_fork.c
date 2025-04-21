/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   take_fork.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manufern <manufern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 12:25:10 by manufern          #+#    #+#             */
/*   Updated: 2025/04/21 15:08:29 by manufern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../filo.h"

static int	is_philosopher_dead_and_unlock(t_filo *data,
	int left_fork, int right_fork, int unlock_print)
{
	if (data->philosopher_dead)
	{
		pthread_mutex_unlock(&data->forks[left_fork]);
		pthread_mutex_unlock(&data->forks[right_fork]);
		if (unlock_print)
			pthread_mutex_unlock(&data->print);
		pthread_mutex_unlock(&data->death_mutex);
		return (1);
	}
	pthread_mutex_unlock(&data->death_mutex);
	return (0);
}

static void	handle_forks_after_take(t_filo *data, int philosopher_id,
	int left_fork, int right_fork, int first_is_left)
{
	if (is_philosopher_dead_and_unlock(data, left_fork, right_fork, 1))
		return ;
	printf("🍴 %ld %d has taken a fork (%s) 🍴\n",
		get_current_time_ms() - data->start_time,
		philosopher_id + 1, first_is_left ? "left" : "right");
	pthread_mutex_unlock(&data->print);

	pthread_mutex_lock(&data->death_mutex);
	if (is_philosopher_dead_and_unlock(data, left_fork, right_fork, 0))
		return ;
	ft_eat(data, philosopher_id, left_fork, right_fork);
}

static void	take_forks(t_filo *data, int philosopher_id,
	int left_fork, int right_fork, int first_is_left)
{
	int	first = first_is_left ? left_fork : right_fork;
	int	second = first_is_left ? right_fork : left_fork;

	pthread_mutex_lock(&data->forks[first]);
	pthread_mutex_lock(&data->print);
	pthread_mutex_lock(&data->death_mutex);
	if (data->philosopher_dead)
	{
		pthread_mutex_unlock(&data->forks[first]);
		pthread_mutex_unlock(&data->print);
		pthread_mutex_unlock(&data->death_mutex);
		return ;
	}
	pthread_mutex_unlock(&data->death_mutex);
	printf("🍴 %ld %d has taken a fork (%s) 🍴\n",
		get_current_time_ms() - data->start_time,
		philosopher_id + 1, first_is_left ? "left" : "right");
	pthread_mutex_unlock(&data->print);

	pthread_mutex_lock(&data->forks[second]);
	pthread_mutex_lock(&data->print);
	pthread_mutex_lock(&data->death_mutex);
	handle_forks_after_take(data, philosopher_id, left_fork, right_fork, !first_is_left);
}

void	ft_take_fork(t_filo *philo, int philosopher_id,
	int left_fork, int right_fork)
{
	t_filo	*data;

	data = (t_filo *)philo;
	if ((philosopher_id + 1) % 2 == 0)
		take_forks(data, philosopher_id, left_fork, right_fork, 1);
	else
	{
		usleep(1000);
		take_forks(data, philosopher_id, left_fork, right_fork, 0);
	}
}

