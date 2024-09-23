/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manufern <manufern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 10:45:05 by manufern          #+#    #+#             */
/*   Updated: 2024/09/23 16:58:22 by manufern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../filo.h"

long get_current_time_ms()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
}

// Función del filósofo


// Crear los hilos para los filósofos
void create_thread_philos(t_filo *filo)
{
    int i = 0;
    pthread_t monitor_thread;

    filo->philos = malloc(sizeof(pthread_t) * filo->number_of_philosophers);
    if (!filo->philos)
    {
        perror("Malloc failed for philosopher threads");
        exit(EXIT_FAILURE);
    }

    // Crear los hilos de los filósofos
    while (i < filo->number_of_philosophers)
    {
        if (pthread_create(&filo->philos[i], NULL, philosopher, (void *)filo) != 0)
        {
            perror("Failed to create philosopher thread");
            exit(EXIT_FAILURE);
        }
        i++;
    }

    // Crear hilo para el monitor
    if (pthread_create(&monitor_thread, NULL, monitor_philosophers, (void *)filo) != 0)
    {
        perror("Failed to create monitor thread");
        exit(EXIT_FAILURE);
    }

    // Esperar a que los hilos de los filósofos terminen
    i = 0;
    while (i < filo->number_of_philosophers)
    {
        if (pthread_join(filo->philos[i], NULL) != 0)
        {
            perror("Failed to join philosopher thread");
            exit(EXIT_FAILURE);
        }
        i++;
    }

    // Esperar a que el hilo monitor termine
    if (pthread_join(monitor_thread, NULL) != 0)
    {
        perror("Failed to join monitor thread");
        exit(EXIT_FAILURE);
    }
}

// Inicializar la estructura del filósofo
void init_filo_struct(t_filo **filo, char **argv)
{
    int i = 0;

    *filo = malloc(sizeof(t_filo));
    if (!*filo)
    {
        perror("Malloc failed for t_filo");
        exit(EXIT_FAILURE);
    }

    (*filo)->number_of_philosophers = atol(argv[1]);
    (*filo)->time_to_die = atol(argv[2]);
    (*filo)->time_to_eat = atol(argv[3]);
    (*filo)->time_to_sleep = atol(argv[4]);
    if (argv[5])
    (*filo)->number_of_times_each_philosopher_must_eat = atol(argv[5]);
	else
		(*filo)->number_of_times_each_philosopher_must_eat = -1;

    // Inicializar mutexes para los tenedores
    (*filo)->forks = malloc(sizeof(pthread_mutex_t) * (*filo)->number_of_philosophers);
    (*filo)->last_meal_mutex = malloc(sizeof(pthread_mutex_t) * (*filo)->number_of_philosophers);
    (*filo)->laps = malloc(sizeof(int) * (*filo)->number_of_philosophers);
    (*filo)->eat = malloc(sizeof(int) * (*filo)->number_of_philosophers);
    if (!(*filo)->forks || !(*filo)->last_meal_mutex || !(*filo)->laps || !(*filo)->eat)
    {
        perror("Malloc failed for forks or last_meal_mutex");
        exit(EXIT_FAILURE);
    }
    
    // Inicializar tiempos de última comida
    (*filo)->last_meal_time = malloc(sizeof(long) * (*filo)->number_of_philosophers);
    if (!(*filo)->last_meal_time)
    {
        perror("Malloc failed for last_meal_time");
        exit(EXIT_FAILURE);
    }
    
    while (i < (*filo)->number_of_philosophers)
    {
        if (pthread_mutex_init(&(*filo)->forks[i], NULL) != 0 ||
            pthread_mutex_init(&(*filo)->last_meal_mutex[i], NULL) != 0)
        {
            perror("Mutex init failed");
            exit(EXIT_FAILURE);
        }
        (*filo)->last_meal_time[i] = get_current_time_ms();
        i++;
    }

    // Inicializar el mutex para impresión
    if (pthread_mutex_init(&(*filo)->print, NULL) != 0)
    {
        perror("Mutex init failed");
        exit(EXIT_FAILURE);
    }
    (*filo)->init_program = get_current_time_ms();
    create_thread_philos(*filo);

    // Destruir mutexes y liberar memoria
    i = 0;
    while (i < (*filo)->number_of_philosophers)
    {
        pthread_mutex_destroy(&(*filo)->forks[i]);
        pthread_mutex_destroy(&(*filo)->last_meal_mutex[i]);
        i++;
    }
    free((*filo)->forks);
    free((*filo)->last_meal_mutex);
    free((*filo)->last_meal_time);
    pthread_mutex_destroy(&(*filo)->print);
    free(*filo);
}

// Función para inicializar la estructura y comenzar la simulación
void init_filo(char **argv)
{
    t_filo *filo;

    init_filo_struct(&filo, argv);
}

// Función principal
int main(int argc, char **argv)
{
    if (argc < 5 || argc > 6)
    {
        printf("Error: Invalid number of arguments\n");
        return (1);
    }
    init_filo(argv);
    return (0);
}
