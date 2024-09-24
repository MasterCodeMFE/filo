/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   life_philosopher.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manufern <manufern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 09:53:56 by manufern          #+#    #+#             */
/*   Updated: 2024/09/24 16:27:56 by manufern         ###   ########.fr       */
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
            // Verificar si el filósofo ha comido el número de veces necesario
            pthread_mutex_lock(&filo->laps_mutex[i]); // Proteger el acceso a laps
            if(filo->laps[i] == filo->number_of_times_each_philosopher_must_eat && filo->number_of_times_each_philosopher_must_eat != -1)
            {
                pthread_mutex_unlock(&filo->laps_mutex[i]);
                i++;
                continue;
            }
            pthread_mutex_unlock(&filo->laps_mutex[i]);

            // Bloquear el mutex para acceder al tiempo de la última comida
            pthread_mutex_lock(&filo->last_meal_mutex[i]);
            current_time = get_current_time_ms();
            
            // Comprobar si el filósofo ha excedido el tiempo permitido sin comer
            if (current_time - filo->last_meal_time[i] > filo->time_to_die)
            {
                // Bloquear mutex para marcar que un filósofo ha muerto
                pthread_mutex_lock(&filo->dead_mutex);
                filo->is_dead = 1;
                pthread_mutex_unlock(&filo->dead_mutex);

                // Imprimir que el filósofo ha muerto
                pthread_mutex_lock(&filo->print);
                printf("💀 %ld %d has died 💀\n", current_time - filo->init_program, i + 1);
                pthread_mutex_unlock(&filo->print);

                // Liberar el mutex del tiempo de la última comida antes de salir
                pthread_mutex_unlock(&filo->last_meal_mutex[i]);
                return (NULL); // Terminar el hilo monitor
            }
            pthread_mutex_unlock(&filo->last_meal_mutex[i]);
            all_finished = 0; // Al menos un filósofo no ha terminado
            i++;
        }

        // Si todos han comido la cantidad necesaria, salir del bucle
        if (all_finished)
        {
            break; // Terminar el hilo monitor
        }

        // Dormir por un corto periodo para evitar alta carga de CPU
        usleep(100); // Aumentar el tiempo de descanso para evitar alta carga
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

    if (filo->number_of_philosophers == 1)
        return (NULL);

    if (filo->number_of_times_each_philosopher_must_eat == -1)
        laps = -1;

    if(philosopher_id % 2 != 0)
        usleep(200);
    pthread_mutex_lock(&filo->laps_mutex[philosopher_id]);
    filo->laps[philosopher_id] = laps;
    pthread_mutex_unlock(&filo->laps_mutex[philosopher_id]);

    while (laps == -1 || laps < filo->number_of_times_each_philosopher_must_eat)
    {
        filo->eat[philosopher_id] = 0;
        timestamp = get_current_time_ms();

        // Verificación temprana de si el filósofo ha muerto antes de tomar los tenedores
        pthread_mutex_lock(&filo->dead_mutex);
        if (filo->is_dead == 1)
        {
            pthread_mutex_unlock(&filo->dead_mutex);
            return (NULL);
        }
        pthread_mutex_unlock(&filo->dead_mutex);
        if (philosopher_id + 1 == filo->number_of_philosophers)
            pthread_mutex_lock(&filo->pick_fork_mutex[0]);
        else
            pthread_mutex_lock(&filo->pick_fork_mutex[philosopher_id + 1]);
        pthread_mutex_lock(&filo->pick_fork_mutex[philosopher_id - 1]);
        if (filo->number_of_philosophers % 2 != 0) // Si el número de filósofos es impar
        {
            pthread_mutex_lock(&filo->forks[left_fork]);
            pthread_mutex_lock(&filo->forks[right_fork]);
        }
        else // Si el número de filósofos es par
        {
            // Alternar el orden en que toman los tenedores
            if (philosopher_id % 2 == 0) // Filósofos con ID par
            {
                pthread_mutex_lock(&filo->forks[left_fork]);
                pthread_mutex_lock(&filo->forks[right_fork]);
            }
            else // Filósofos con ID impar
            {
                pthread_mutex_lock(&filo->forks[right_fork]);
                pthread_mutex_lock(&filo->forks[left_fork]);
            }
        }
        pthread_mutex_unlock(&filo->pick_fork_mutex[philosopher_id - 1]);
        if (philosopher_id + 1 == filo->number_of_philosophers)
            pthread_mutex_unlock(&filo->pick_fork_mutex[0]);
        else
            pthread_mutex_unlock(&filo->pick_fork_mutex[philosopher_id + 1]);

        // Comer
        pthread_mutex_lock(&filo->dead_mutex);
        if (filo->is_dead == 0)
        {
            pthread_mutex_unlock(&filo->dead_mutex);
            pthread_mutex_lock(&filo->print);

            // Bloquear el mutex para actualizar el tiempo de la última comida
            pthread_mutex_lock(&filo->last_meal_mutex[philosopher_id]);
            filo->last_meal_time[philosopher_id] = get_current_time_ms(); // Actualizar el tiempo de la última comida
            pthread_mutex_unlock(&filo->last_meal_mutex[philosopher_id]);

            filo->eat[philosopher_id] = 1;
            printf("🍽️  %ld %d is eating 🍽️\n", filo->last_meal_time[philosopher_id] - filo->init_program, philosopher_id + 1);
            pthread_mutex_unlock(&filo->print);

            my_usleep(filo->time_to_eat * 1000); // Simular el tiempo comiendo
        }
        else
        {
            pthread_mutex_unlock(&filo->dead_mutex);
        }

        // Dejar los tenedores
        pthread_mutex_unlock(&filo->forks[right_fork]);
        pthread_mutex_unlock(&filo->forks[left_fork]);

        // Dormir y repetir
        timestamp = get_current_time_ms();
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
        {
            pthread_mutex_unlock(&filo->dead_mutex);
        }

        // Incrementar las vueltas de comida si hay un límite
        pthread_mutex_lock(&filo->dead_mutex);
        if (filo->is_dead == 0)
        {
            pthread_mutex_unlock(&filo->dead_mutex);
            pthread_mutex_lock(&filo->print);
            printf("🧑 %ld %d is thinking 🧑\n", timestamp - filo->init_program, philosopher_id + 1);
            pthread_mutex_unlock(&filo->print);
        }
        else
        {
            pthread_mutex_unlock(&filo->dead_mutex);
        }

        // Incrementar el número de comidas realizadas si no es infinito
        if (laps != -1)
            laps++;

        // Actualizar el número de vueltas del filósofo
        pthread_mutex_lock(&filo->laps_mutex[philosopher_id]);
        filo->laps[philosopher_id] = laps;
        pthread_mutex_unlock(&filo->laps_mutex[philosopher_id]);
    }
    return (NULL);
}

