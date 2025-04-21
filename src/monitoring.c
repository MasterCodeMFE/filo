/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manufern <manufern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 12:21:22 by manufern          #+#    #+#             */
/*   Updated: 2025/04/21 15:04:39 by manufern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../filo.h"

void	ft_dead(t_filo *philo, int i, long current_time)
{
	t_filo	*data = (t_filo *)philo;

	// Bloquear el mutex de muerte solo cuando se actualiza el estado de la muerte
	pthread_mutex_lock(&data->death_mutex);
	if (!data->philosopher_dead)  // Solo marcamos la muerte si aún no está muerta
	{
		data->philosopher_dead = 1;
		pthread_mutex_unlock(&data->death_mutex);

		pthread_mutex_lock(&data->print);
		printf("💀 %ld %d has died 💀\n", current_time - data->start_time, i + 1);
		pthread_mutex_unlock(&data->print);
	}
	else
	{
		pthread_mutex_unlock(&data->death_mutex);
	}
}

void	*monitoring_aux(t_filo *philo, int i,
	int *finished_philosophers, pthread_mutex_t *finished_mutex)
{
	t_filo	*data = (t_filo *)philo;
	long	current_time;
	
	while (++i < data->number_philo)
	{
		// Solo continuar si el filósofo no ha comido suficientes veces
		if (data->number_of_times_eat != -1 && data->number_of_eats[i] >= data->number_of_times_eat)
		{
			pthread_mutex_lock(finished_mutex);
			(*finished_philosophers)++;
			pthread_mutex_unlock(finished_mutex);
			continue ;
		}

		current_time = get_current_time_ms();
		// Bloquear el mutex de comida solo si realmente se va a comprobar la última comida
		pthread_mutex_lock(&data->meal_time_mutex);
		if (current_time - data->last_meal_time[i] > data->time_to_die)
		{
			pthread_mutex_unlock(&data->meal_time_mutex);  // Desbloquear antes de morir
			ft_dead(data, i, current_time);
			return (NULL); // Regresar inmediatamente
		}
		pthread_mutex_unlock(&data->meal_time_mutex);
	}
	return (NULL);
}

void	*monitoring(void *philo)
{
	t_filo			*data = (t_filo *)philo;
	int				i;
	int				finished_philosophers = 0;
	pthread_mutex_t	finished_mutex = PTHREAD_MUTEX_INITIALIZER;

	while (1)
	{
		i = -1;
		monitoring_aux(data, i, &finished_philosophers, &finished_mutex);

		// Solo verificar si los filósofos han terminado cuando es necesario
		pthread_mutex_lock(&finished_mutex);
		if (finished_philosophers >= data->number_philo)
		{
			pthread_mutex_unlock(&finished_mutex);
			break ;
		}
		pthread_mutex_unlock(&finished_mutex);

		// Verificar si algún filósofo ha muerto sin bloquear innecesariamente
		pthread_mutex_lock(&data->death_mutex);
		if (data->philosopher_dead)
		{
			pthread_mutex_unlock(&data->death_mutex);
			return (NULL); // Salir inmediatamente si un filósofo está muerto
		}
		pthread_mutex_unlock(&data->death_mutex);

		usleep(100);  // Pausa pequeña para evitar un uso excesivo de CPU
	}
	return (NULL);
}
