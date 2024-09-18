/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manufern <manufern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 10:45:05 by manufern          #+#    #+#             */
/*   Updated: 2024/09/18 14:48:56 by manufern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../filo.h"
#include <sys/time.h>

long get_current_time_ms()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
}

// Función del filósofo
void *philosopher(void *arg)
{
    t_filo *filo;
    static int i = 0;
    int left_fork;
    int right_fork;
    long timestamp;
    int	laps;

    filo = (t_filo *)arg;
	laps = 0;
    int philosopher_id = i++;

    left_fork = philosopher_id;
    right_fork = (philosopher_id + 1) % filo->number_of_philosophers;
	if (filo->number_of_times_each_philosopher_must_eat == -1)
		laps = -1;	
    while (laps == -1 || laps < filo->number_of_times_each_philosopher_must_eat)
    {
        filo->eat = 0;
        timestamp = get_current_time_ms();

        // Pensar
        pthread_mutex_lock(&filo->print);
        printf("🧑 %ld %d is thinking 🧑\n", timestamp, philosopher_id + 1);
        pthread_mutex_unlock(&filo->print);

        // Verificar si ha pasado demasiado tiempo desde la última comida
        pthread_mutex_lock(&filo->last_meal_mutex[philosopher_id]);
        if (get_current_time_ms() - filo->last_meal_time[philosopher_id] > filo->time_to_die)
        {
            // Si ha pasado más tiempo del permitido sin comer, el filósofo muere
            pthread_mutex_lock(&filo->print);
            printf("💀 %ld %d has died 💀\n", timestamp, philosopher_id + 1);
            pthread_mutex_unlock(&filo->print);
            pthread_mutex_unlock(&filo->last_meal_mutex[philosopher_id]);
            break;
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
        pthread_mutex_lock(&filo->print);
        filo->eat = 1;
        filo->last_meal_time[philosopher_id] = get_current_time_ms(); // Actualizar el tiempo de la última comida
        printf("🍽️ %ld %d is eating 🍽️\n", filo->last_meal_time[philosopher_id], philosopher_id + 1);
        pthread_mutex_unlock(&filo->print);

        usleep(filo->time_to_eat * 1000); // Simular el tiempo comiendo

        // Dejar los tenedores
        pthread_mutex_unlock(&filo->forks[right_fork]);
        pthread_mutex_unlock(&filo->forks[left_fork]);

        // Dormir y repetir
        timestamp = get_current_time_ms();
        pthread_mutex_lock(&filo->print);
        printf("🛌 %ld %d is sleeping 🛌\n", timestamp, philosopher_id + 1);
        pthread_mutex_unlock(&filo->print);

        usleep(filo->time_to_sleep * 1000); // Simular el tiempo durmiendo
		if (laps != -1)
			laps++;
    }

    return (NULL);
}

// Crear los hilos para los filósofos
void create_thread_philos(t_filo *filo)
{
    int i = 0;

    filo->philos = malloc(sizeof(pthread_t) * filo->number_of_philosophers);
    if (!filo->philos)
    {
        perror("Malloc failed for philosopher threads");
        exit(EXIT_FAILURE);
    }

    while (i < filo->number_of_philosophers)
    {
        if (pthread_create(&filo->philos[i], NULL, philosopher, (void *)filo) != 0)
        {
            perror("Failed to create thread");
            exit(EXIT_FAILURE);
        }
        i++;
    }

    i = 0;
    while (i < filo->number_of_philosophers)
    {
        if (pthread_join(filo->philos[i], NULL) != 0)
        {
            perror("Failed to join thread");
            exit(EXIT_FAILURE);
        }
        i++;
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
    if (!(*filo)->forks || !(*filo)->last_meal_mutex)
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
// Estructura que contiene el mutex y la variable compartida
/* typedef struct {
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
	pthread_t threads[32754];
	thread_data_t data;
	
	// Inicializar la estructura
	data.shared_counter = 0;

	// Inicializar el mutex
	if (pthread_mutex_init(&data.mutex, NULL) != 0) {
		perror("Mutex initialization failed");
		return EXIT_FAILURE;
	}

	// Crear los hilos
	for (int i = 0; i < 32754; i++) {
		if (pthread_create(&threads[i], NULL, thread_function, &data) != 0) {
			perror("Thread creation failed");
			pthread_mutex_destroy(&data.mutex);
			return EXIT_FAILURE;
		}
	}

	// Esperar a que todos los hilos terminen
	for (int i = 0; i < 32754; i++) {
		pthread_join(threads[i], NULL);
	}

	// Imprimir el resultado final
	printf("Final value of shared_counter: %d\n", data.shared_counter);

	// Destruir el mutex
	pthread_mutex_destroy(&data.mutex);

	return EXIT_SUCCESS;
} */