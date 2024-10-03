/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   take_fork.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manufern <manufern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 12:25:10 by manufern          #+#    #+#             */
/*   Updated: 2024/10/03 13:21:58 by manufern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../filo.h"

void	take_fork_odd_aux(t_filo *philo, int philosopher_id,
		int left_fork, int right_fork)
{
	t_filo	*data;

	data = (t_filo *)philo;
	if (data->philosopher_dead)
	{
		pthread_mutex_unlock(&data->forks[right_fork]);
		pthread_mutex_unlock(&data->forks[left_fork]);
		pthread_mutex_unlock(&data->print);
		pthread_mutex_unlock(&data->death_mutex);
		return ;
	}
	pthread_mutex_unlock(&data->death_mutex);
	printf("🍴 %ld %d has taken a fork (left) 🍴\n",
		get_current_time_ms() - data->start_time, philosopher_id + 1);
	pthread_mutex_unlock(&data->print);
	pthread_mutex_lock(&data->death_mutex);
	if (data->philosopher_dead)
	{
		pthread_mutex_unlock(&data->forks[right_fork]);
		pthread_mutex_unlock(&data->forks[left_fork]);
		pthread_mutex_unlock(&data->death_mutex);
		return ;
	}
	pthread_mutex_unlock(&data->death_mutex);
	ft_eat(data, philosopher_id, left_fork, right_fork);
}

void	take_fork_odd(t_filo *philo, int philosopher_id,
	int left_fork, int right_fork)
{
	t_filo	*data;

	data = (t_filo *)philo;
	pthread_mutex_lock(&data->forks[right_fork]);
	pthread_mutex_lock(&data->print);
	pthread_mutex_lock(&data->death_mutex);
	if (data->philosopher_dead)
	{
		pthread_mutex_unlock(&data->forks[right_fork]);
		pthread_mutex_unlock(&data->print);
		pthread_mutex_unlock(&data->death_mutex);
		return ;
	}
	pthread_mutex_unlock(&data->death_mutex);
	printf("🍴 %ld %d has taken a fork (right) 🍴\n",
		get_current_time_ms() - data->start_time, philosopher_id + 1);
	pthread_mutex_unlock(&data->print);
	pthread_mutex_lock(&data->forks[left_fork]);
	pthread_mutex_lock(&data->print);
	pthread_mutex_lock(&data->death_mutex);
}

void	take_fork_even_aux(t_filo *philo, int philosopher_id,
	int left_fork, int right_fork)
{
	t_filo	*data;

	data = (t_filo *)philo;
	if (data->philosopher_dead == 1)
	{
		pthread_mutex_unlock(&data->forks[left_fork]);
		pthread_mutex_unlock(&data->forks[right_fork]);
		pthread_mutex_unlock(&data->print);
		pthread_mutex_unlock(&data->death_mutex);
		return ;
	}
	pthread_mutex_unlock(&data->death_mutex);
	printf("🍴 %ld %d has taken a fork (right) 🍴\n",
		get_current_time_ms() - data->start_time, philosopher_id + 1);
	pthread_mutex_unlock(&data->print);
	pthread_mutex_lock(&data->death_mutex);
	if (data->philosopher_dead == 1)
	{
		pthread_mutex_unlock(&data->forks[left_fork]);
		pthread_mutex_unlock(&data->forks[right_fork]);
		pthread_mutex_unlock(&data->death_mutex);
		return ;
	}
	pthread_mutex_unlock(&data->death_mutex);
	ft_eat(data, philosopher_id, left_fork, right_fork);
}

void	take_fork_even(t_filo *philo, int philosopher_id,
	int left_fork, int right_fork)
{
	t_filo	*data;

	data = (t_filo *)philo;
	pthread_mutex_lock(&data->forks[left_fork]);
	pthread_mutex_lock(&data->print);
	pthread_mutex_lock(&data->death_mutex);
	if (data->philosopher_dead == 1)
	{
		pthread_mutex_unlock(&data->forks[left_fork]);
		pthread_mutex_unlock(&data->print);
		pthread_mutex_unlock(&data->death_mutex);
		return ;
	}
	pthread_mutex_unlock(&data->death_mutex);
	printf("🍴 %ld %d has taken a fork (left) 🍴\n",
		get_current_time_ms() - data->start_time, philosopher_id + 1);
	pthread_mutex_unlock(&data->print);
	pthread_mutex_lock(&data->forks[right_fork]);
	pthread_mutex_lock(&data->print);
	pthread_mutex_lock(&data->death_mutex);
}

void	ft_take_fork(t_filo *philo, int philosopher_id,
	int left_fork, int right_fork)
{
	t_filo	*data;

	data = (t_filo *)philo;
	if (philosopher_id + 1 % 2 == 0)
	{
		take_fork_even(data, philosopher_id, left_fork, right_fork);
		take_fork_even_aux(data, philosopher_id, left_fork, right_fork);
	}
	else
	{
		usleep(1000);
		take_fork_odd(data, philosopher_id, left_fork, right_fork);
		take_fork_odd_aux(data, philosopher_id, left_fork, right_fork);
	}
}
