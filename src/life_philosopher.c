/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   life_philosopher.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manufern <manufern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 09:53:56 by manufern          #+#    #+#             */
/*   Updated: 2024/09/23 17:00:06 by manufern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../filo.h"

void *monitor_philosophers(void *arg)
{
    t_filo *filo;
    int i;
    long current_time;

    filo = (t_filo *)arg;
    while (filo->is_dead == 0) // El bucle sigue hasta que un filósofo muere
    {
        i = 0;
        while (i < filo->number_of_philosophers)
        {
            pthread_mutex_lock(&filo->last_meal_mutex[i]);
            // Si el filósofo ha comido las veces necesarias, no lo monitorees más
            if (filo->laps[i] >= filo->number_of_times_each_philosopher_must_eat)
            {
                pthread_mutex_unlock(&filo->last_meal_mutex[i]);
                i++;
                continue;
            }

            current_time = get_current_time_ms();
            if (current_time - filo->last_meal_time[i] > filo->time_to_die)
            {
                filo->is_dead = 1;
                pthread_mutex_lock(&filo->print);
                printf("💀 %ld %d has died 💀\n", current_time - filo->init_program, i + 1);
                pthread_mutex_unlock(&filo->print);
                pthread_mutex_unlock(&filo->last_meal_mutex[i]);
                break; // Sal del bucle interno si un filósofo muere
            }
            pthread_mutex_unlock(&filo->last_meal_mutex[i]);
            i++;
        }
        usleep(1000); // Pequeño descanso para evitar alta carga de CPU
    }
    return (NULL);
}

void *philosopher(void *arg)
{
    t_filo *filo;
    int left_fork;
    int right_fork;
    int philosopher_id;
    long timestamp;
    int laps;

    filo = (t_filo *)arg;
    laps = 0;

    // Asignación segura de philosopher_id usando un mutex
    pthread_mutex_lock(&filo->id_mutex);
    philosopher_id = filo->philosopher_id++;
    pthread_mutex_unlock(&filo->id_mutex);

    left_fork = philosopher_id;
    right_fork = (philosopher_id + 1) % filo->number_of_philosophers;

    if (filo->number_of_times_each_philosopher_must_eat == -1)
	{
        laps = -1;
	}
	pthread_mutex_lock(&filo->id_mutex);
	filo->laps[philosopher_id] = laps;
	pthread_mutex_unlock(&filo->id_mutex);
    while (laps == -1 || laps < filo->number_of_times_each_philosopher_must_eat)
    {
        filo->eat[philosopher_id] = 0;
        timestamp = get_current_time_ms();

        // Verificar si ha pasado demasiado tiempo desde la última comida
        pthread_mutex_lock(&filo->last_meal_mutex[philosopher_id]);
        if (get_current_time_ms() - filo->last_meal_time[philosopher_id] > filo->time_to_die || filo->number_of_philosophers < 2)
        {
            // Si ha pasado más tiempo del permitido sin comer, el filósofo muere
            if (filo->is_dead == 0)
            {
                filo->is_dead = 1; // Asegúrate de que la variable is_dead se actualiza
                pthread_mutex_lock(&filo->print);
                printf("💀 %ld %d has died 💀\n", timestamp - filo->init_program, philosopher_id + 1);
                pthread_mutex_unlock(&filo->print);
                pthread_mutex_unlock(&filo->last_meal_mutex[philosopher_id]);
            }
            return (NULL); // Sale del ciclo, el filósofo ha muerto
        }
        pthread_mutex_unlock(&filo->last_meal_mutex[philosopher_id]);

        // Intentar tomar los tenedores
        if (philosopher_id % 2 == 0)
        {
            pthread_mutex_lock(&filo->forks[left_fork]);
            pthread_mutex_lock(&filo->forks[right_fork]);
        }
        else
        {
            pthread_mutex_lock(&filo->forks[right_fork]);
            pthread_mutex_lock(&filo->forks[left_fork]);
        }

        // Comer
        if (filo->is_dead == 0)
        {
            pthread_mutex_lock(&filo->print);
            filo->eat[philosopher_id] = 1;
            filo->last_meal_time[philosopher_id] = get_current_time_ms(); // Actualizar el tiempo de la última comida
            printf("🍽️  %ld %d is eating 🍽️\n", filo->last_meal_time[philosopher_id] - filo->init_program, philosopher_id + 1);
            pthread_mutex_unlock(&filo->print);

            my_usleep(filo->time_to_eat * 1000); // Simular el tiempo comiendo
        }

        // Dejar los tenedores
        pthread_mutex_unlock(&filo->forks[right_fork]);
        pthread_mutex_unlock(&filo->forks[left_fork]);

        // Dormir y repetir
        timestamp = get_current_time_ms();
        timestamp = get_current_time_ms();
        if (filo->is_dead == 0 && filo-> eat[philosopher_id] == 1)
        {
            pthread_mutex_lock(&filo->print);
            printf("🛌 %ld %d is sleeping 🛌\n", timestamp - filo->init_program, philosopher_id + 1);
            pthread_mutex_unlock(&filo->print);
            my_usleep(filo->time_to_sleep * 1000);
        }

        // Incrementar las vueltas de comida si hay un límite
        if (filo->is_dead == 0)
        {
            pthread_mutex_lock(&filo->print);
            printf("🧑 %ld %d is thinking 🧑\n", timestamp - filo->init_program, philosopher_id + 1);
            pthread_mutex_unlock(&filo->print);
        }
        if (laps != -1)
            laps ++;
		filo->laps[philosopher_id] = laps;
    }

    return (NULL);
}
