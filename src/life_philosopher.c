/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   life_philosopher.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manufern <manufern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 09:53:56 by manufern          #+#    #+#             */
/*   Updated: 2024/09/26 10:54:48 by manufern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../filo.h"

void *monitor_philosophers(void *arg)
{
	t_filo *filo = (t_filo *)arg;
	int i;
	long current_time;
	int all_finished;

	while (1)
	{
		i = 0;
		all_finished = 1; // Asumir que todos han terminado

		while (i < filo->number_of_philosophers)
		{
			pthread_mutex_lock(&filo->laps_mutex[i]); // Proteger el acceso a laps
			if (filo->laps[i] == filo->number_of_times_each_philosopher_must_eat && filo->number_of_times_each_philosopher_must_eat != -1)
			{
				pthread_mutex_unlock(&filo->laps_mutex[i]);
				i++;
				continue;
			}
			pthread_mutex_unlock(&filo->laps_mutex[i]);

			// Bloquear el mutex para acceder al tiempo de la última comida
			pthread_mutex_lock(&filo->last_meal_mutex[i]);
			current_time = get_current_time_ms();
			
			if (current_time - filo->last_meal_time[i] > filo->time_to_die)
			{
				pthread_mutex_lock(&filo->dead_mutex);
				filo->is_dead = 1;
				pthread_mutex_unlock(&filo->dead_mutex);

				pthread_mutex_lock(&filo->print);
				printf("💀 %ld %d has died 💀\n", current_time - filo->init_program, i + 1);
				pthread_mutex_unlock(&filo->print);
				pthread_mutex_unlock(&filo->last_meal_mutex[i]);
				return (NULL); // Terminar el hilo monitor
			}
			pthread_mutex_unlock(&filo->last_meal_mutex[i]);
			all_finished = 0; // Al menos un filósofo no ha terminado
			i++;
		}

		if (all_finished)
			break; // Terminar el hilo monitor
	}
	return (NULL);
}

void ft_think(t_filo *filo, int philosopher_id)
{
	long timestamp = get_current_time_ms();
	
	pthread_mutex_lock(&filo->dead_mutex);
	if (filo->is_dead == 0)
	{
		pthread_mutex_unlock(&filo->dead_mutex);
		pthread_mutex_lock(&filo->print);
		printf("🧑 %ld %d is thinking 🧑\n", timestamp - filo->init_program, philosopher_id + 1);
		pthread_mutex_unlock(&filo->print);
	}
	else
		pthread_mutex_unlock(&filo->dead_mutex);
	
	size_t time = filo->time_to_die - (filo->time_to_eat * 2);
	if (time > 0)
		my_usleep(time * 0.42);
}

void ft_sleep(t_filo *filo, int philosopher_id)
{
	long timestamp = get_current_time_ms();
	
	pthread_mutex_lock(&filo->dead_mutex);
	if (filo->is_dead == 0 && filo->eat[philosopher_id] == 1)
	{
		pthread_mutex_unlock(&filo->dead_mutex);
		pthread_mutex_lock(&filo->print);
		printf("🛌 %ld %d is sleeping 🛌\n", timestamp - filo->init_program, philosopher_id + 1);
		pthread_mutex_unlock(&filo->print);
		my_usleep(filo->time_to_sleep * 1000);
	}
	else
		pthread_mutex_unlock(&filo->dead_mutex);
}

void ft_eat(t_filo *filo, int philosopher_id)
{
	pthread_mutex_lock(&filo->dead_mutex);
	if (filo->is_dead == 0)
	{
		pthread_mutex_unlock(&filo->dead_mutex);
		pthread_mutex_lock(&filo->print);

		pthread_mutex_lock(&filo->last_meal_mutex[philosopher_id]);
		filo->last_meal_time[philosopher_id] = get_current_time_ms(); // Actualizar el tiempo de la última comida
		pthread_mutex_unlock(&filo->last_meal_mutex[philosopher_id]);

		filo->eat[philosopher_id] = 1;
		pthread_mutex_lock(&filo->last_meal_mutex[philosopher_id]);
		printf("🍽️  %ld %d is eating 🍽️\n", filo->last_meal_time[philosopher_id] - filo->init_program, philosopher_id + 1);
		pthread_mutex_unlock(&filo->last_meal_mutex[philosopher_id]);
		pthread_mutex_unlock(&filo->print);
		my_usleep(filo->time_to_eat * 1000); // Simular el tiempo comiendo
	}
	else
		pthread_mutex_unlock(&filo->dead_mutex);
}

void *philosopher(void *arg)
{
	t_filo *filo = (t_filo *)arg;
	int left_fork;
	int right_fork;
	int philosopher_id;
	int laps = 0;

	pthread_mutex_lock(&filo->id_mutex);
	philosopher_id = filo->philosopher_id++;
	pthread_mutex_unlock(&filo->id_mutex);

	left_fork = philosopher_id;
	right_fork = (philosopher_id + 1) % filo->number_of_philosophers;

	if (filo->number_of_philosophers == 1)
	{
		printf("🍴 %ld %d has taken a fork 🍴\n", get_current_time_ms() - filo->init_program, philosopher_id + 1);
		return (NULL);
	}
	if (filo->number_of_times_each_philosopher_must_eat == -1)
		laps = -1;

	pthread_mutex_lock(&filo->laps_mutex[philosopher_id]);
	filo->laps[philosopher_id] = laps;
	pthread_mutex_unlock(&filo->laps_mutex[philosopher_id]);
	if((philosopher_id + 1) % 2 == 0)
		my_usleep(1000);
	while (laps == -1 || laps < filo->number_of_times_each_philosopher_must_eat)
	{
		filo->eat[philosopher_id] = 0;

		pthread_mutex_lock(&filo->dead_mutex);
		if (filo->is_dead == 1) // Si hay un filósofo muerto, salir del ciclo.
		{
			pthread_mutex_unlock(&filo->dead_mutex);
			return (NULL);
		}
		pthread_mutex_unlock(&filo->dead_mutex);

		// Asegurar el orden de bloqueo de tenedores
		pthread_mutex_lock(&filo->dead_mutex);
		if (filo->is_dead == 1) // Verificar de nuevo antes de intentar tomar un tenedor
		{
			pthread_mutex_unlock(&filo->dead_mutex);
			return (NULL);
		}
		pthread_mutex_unlock(&filo->dead_mutex);
		
		// Bloqueo de tenedores y otras acciones (comer, dormir, pensar) solo si no están muertos
		if (philosopher_id % 2 == 0)
		{
			pthread_mutex_lock(&filo->forks[left_fork]);
			pthread_mutex_lock(&filo->print);
			printf("🍴 %ld %d has taken a fork 🍴\n", get_current_time_ms() - filo->init_program, philosopher_id + 1);
			pthread_mutex_unlock(&filo->print);
			pthread_mutex_lock(&filo->forks[right_fork]);
			pthread_mutex_lock(&filo->print);
			printf("🍴 %ld %d has taken a fork 🍴\n", get_current_time_ms() - filo->init_program, philosopher_id + 1);
			pthread_mutex_unlock(&filo->print);
		}
		// Comer
		ft_eat(filo, philosopher_id);
		// Dejar los tenedores
		pthread_mutex_unlock(&filo->forks[right_fork]);
		pthread_mutex_unlock(&filo->forks[left_fork]);

		// Dormir y pensar
		ft_sleep(filo, philosopher_id);
		ft_think(filo, philosopher_id);

		// Incrementar el número de comidas realizadas
		if (laps != -1)
			laps++;

		pthread_mutex_lock(&filo->laps_mutex[philosopher_id]);
		filo->laps[philosopher_id] = laps;
		pthread_mutex_unlock(&filo->laps_mutex[philosopher_id]);
	}
	return (NULL);
}
