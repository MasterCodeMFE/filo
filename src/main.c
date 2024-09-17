/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manufern <manufern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 10:45:05 by manufern          #+#    #+#             */
/*   Updated: 2024/09/17 17:12:41 by manufern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../filo.h"

/* void init_mutex(t_filo *filo)
{
    int i;

    i = 0;
    while (i < filo->number_of_philosophers)
    {
        pthread_mutex_lock(&filo->print);
        printf("Creating mutex forks %i\n", i + 1);
        pthread_mutex_unlock(&filo->print);
        
        if (pthread_mutex_init(&filo->forks[i], NULL) != 0)
        {
            perror("Mutex initialization failed");
            exit(EXIT_FAILURE);
        }
        i++;
    }
}

void init_filo_struct(t_filo **filo, char **argv)
{
    *filo = malloc(sizeof(t_filo));
    if (!*filo)
    {
        perror("Malloc failed for t_filo");
        exit(EXIT_FAILURE);
    }
    
    (*filo)->number_of_philosophers = ft_atoi(argv[1]);
    (*filo)->time_to_die = ft_atoi(argv[2]);
    (*filo)->time_to_eat = ft_atoi(argv[3]);
    (*filo)->time_to_sleep = ft_atoi(argv[4]);
    if (argv[5])
        (*filo)->number_of_times_each_philosopher_must_eat = ft_atoi(argv[5]);
    else
        (*filo)->number_of_times_each_philosopher_must_eat = -1;

    (*filo)->forks = malloc(sizeof(pthread_mutex_t) * (*filo)->number_of_philosophers);
    if (!(*filo)->forks)
    {
        perror("Malloc failed for forks");
        free(*filo);
        exit(EXIT_FAILURE);
    }

    if (pthread_mutex_init(&(*filo)->print, NULL) != 0)
    {
        perror("Mutex initialization failed");
        free((*filo)->forks);
        free(*filo);
        exit(EXIT_FAILURE);
	}
    init_mutex(*filo);
}

void end_philo(t_filo *filo)
{
    int i;

    i = 0;
    while (i < filo->number_of_philosophers)
    {
        pthread_mutex_destroy(&filo->forks[i]);
        i++;
    }
    free(filo->forks);
    free(filo->philos);
    pthread_mutex_destroy(&filo->print);
    free(filo);
}

void init_filo(char **argv)
{
    t_filo *filo;

    init_filo_struct(&filo, argv);
    end_philo(filo);
}

int main(int argc, char **argv)
{
    if (argc < 5 || argc > 6)
    {
        printf("Error: Invalid number of arguments\n");
        return (1);
    }
    init_filo(argv);   
    return (0);
} */
// Estructura que contiene el mutex y la variable compartida
typedef struct {
    int shared_counter;
    pthread_mutex_t mutex;
} thread_data_t;

void* thread_function(void* arg) {
    thread_data_t* data = (thread_data_t*)arg;
    
    // Cada hilo incrementa la variable compartida 100 veces
        pthread_mutex_lock(&data->mutex);
		printf("hilo: %d\n", data->shared_counter + 1);  // Bloquear el mutex antes de acceder a la variable compartida
        data->shared_counter++;            // Acceso a la variable compartida
        pthread_mutex_unlock(&data->mutex); // Desbloquear el mutex después de modificar la variable
    return NULL;
}

int main() {
    pthread_t threads[20000];
    thread_data_t data;
    
    // Inicializar la estructura
    data.shared_counter = 0;

    // Inicializar el mutex
    if (pthread_mutex_init(&data.mutex, NULL) != 0) {
        perror("Mutex initialization failed");
        return EXIT_FAILURE;
    }

    // Crear los hilos
    for (int i = 0; i < 20000; i++) {
        if (pthread_create(&threads[i], NULL, thread_function, &data) != 0) {
            perror("Thread creation failed");
            pthread_mutex_destroy(&data.mutex);
            return EXIT_FAILURE;
        }
    }

    // Esperar a que todos los hilos terminen
    for (int i = 0; i < 20000; i++) {
        pthread_join(threads[i], NULL);
    }

    // Imprimir el resultado final
    printf("Final value of shared_counter: %d\n", data.shared_counter);

    // Destruir el mutex
    pthread_mutex_destroy(&data.mutex);

    return EXIT_SUCCESS;
}