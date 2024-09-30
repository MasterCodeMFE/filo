/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manufern <manufern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 11:11:44 by manufern          #+#    #+#             */
/*   Updated: 2024/09/30 18:12:09 by manufern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../filo.h"

long get_current_time_ms()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
}

void *monitoring(void *philo)
{
    t_filo *data;
    int     i;
    long    current_time;

    data = (t_filo *)philo;
    while (1)
    {
        i = 0;
        while (i < data->number_of_philosophers)
        {
            current_time = get_current_time_ms();
            pthread_mutex_lock(&data->meal_time_mutex);
            if (current_time - data->last_meal_time[i] > data->time_to_die)
            {
                pthread_mutex_unlock(&data->meal_time_mutex); // Desbloquear antes de salir
                pthread_mutex_lock(&data->print);
                printf("💀 %ld %d has died 💀\n", current_time - data->start_time, i + 1);
                pthread_mutex_unlock(&data->print);

                // Marcar que un filósofo ha muerto
                pthread_mutex_lock(&data->death_mutex);
                data->philosopher_dead = 1;
                pthread_mutex_unlock(&data->death_mutex);
                return (NULL); // Salir del hilo de monitoreo
            }
            pthread_mutex_unlock(&data->meal_time_mutex);
            i++;
        }

        // Revisar si ya ha muerto un filósofo
        pthread_mutex_lock(&data->death_mutex);
        if (data->philosopher_dead)
        {
            pthread_mutex_unlock(&data->death_mutex);
            return (NULL);
        }
        pthread_mutex_unlock(&data->death_mutex);
    }
    return (NULL);
}

void ft_think(t_filo *philo, int philosopher_id)
{
	t_filo *data;

	data = (t_filo *)philo;
	pthread_mutex_lock(&data->print);
	printf("🧑 %ld %d is thinking 🧑\n", get_current_time_ms() - data->start_time, philosopher_id + 1);
	pthread_mutex_unlock(&data->print);
	
}

void ft_sleep(t_filo *philo, int philosopher_id)
{
	t_filo *data;

	data = (t_filo *)philo;
	pthread_mutex_lock(&data->print);
	printf("🛌 %ld %d is sleeping 🛌\n", get_current_time_ms() - data->start_time, philosopher_id + 1);
	pthread_mutex_unlock(&data->print);
	my_usleep(data->time_to_sleep * 1000);
}

void ft_eat(t_filo *philo, int philosopher_id, int left_fork, int right_fork)
{
    t_filo *data;

    data = (t_filo *)philo;
    
    // Proteger el acceso a last_meal_time
    pthread_mutex_lock(&data->meal_time_mutex);
    data->last_meal_time[philosopher_id] = get_current_time_ms();
    pthread_mutex_unlock(&data->meal_time_mutex);

    pthread_mutex_lock(&data->print);
    printf("🍽️  %ld %d is eating 🍽️\n", data->last_meal_time[philosopher_id] - data->start_time, philosopher_id + 1);
    pthread_mutex_unlock(&data->print);

    my_usleep(data->time_to_eat * 1000);

    pthread_mutex_unlock(&data->forks[left_fork]);
    pthread_mutex_unlock(&data->forks[right_fork]);

    data->eat[philosopher_id] = 1;
}

void ft_take_fork(t_filo *philo, int philosopher_id, int left_fork, int right_fork)
{
	t_filo *data;

	data = (t_filo *)philo;
	if (philosopher_id % 2 != 0)
	{
		pthread_mutex_lock(&data->forks[left_fork]);
		pthread_mutex_lock(&data->print);
		pthread_mutex_lock(&data->death_mutex);
        if (data->philosopher_dead)
        {
            pthread_mutex_unlock(&data->death_mutex);
            return ; // Terminar el hilo si un filósofo ha muerto
        }
        pthread_mutex_unlock(&data->death_mutex);
		printf("🍴 %ld %d has taken a fork (left) 🍴\n", get_current_time_ms() - data->start_time, philosopher_id + 1);
		pthread_mutex_unlock(&data->print);
		pthread_mutex_lock(&data->forks[right_fork]);
		pthread_mutex_lock(&data->print);
		pthread_mutex_lock(&data->death_mutex);
        if (data->philosopher_dead)
        {
            pthread_mutex_unlock(&data->death_mutex);
            return ; // Terminar el hilo si un filósofo ha muerto
        }
        pthread_mutex_unlock(&data->death_mutex);
		printf("🍴 %ld %d has taken a fork (right) 🍴\n", get_current_time_ms() - data->start_time, philosopher_id + 1);
		pthread_mutex_unlock(&data->print);
		pthread_mutex_lock(&data->death_mutex);
        if (data->philosopher_dead)
        {
            pthread_mutex_unlock(&data->death_mutex);
            return ; // Terminar el hilo si un filósofo ha muerto
        }
        pthread_mutex_unlock(&data->death_mutex);
		ft_eat(data, philosopher_id, left_fork, right_fork);
	}
	else
	{
		pthread_mutex_lock(&data->forks[right_fork]);
		pthread_mutex_lock(&data->print);
		pthread_mutex_lock(&data->death_mutex);
		if (data->philosopher_dead)
        {
            pthread_mutex_unlock(&data->death_mutex);
            return ; // Terminar el hilo si un filósofo ha muerto
        }
		pthread_mutex_unlock(&data->death_mutex);
		printf("🍴 %ld %d has taken a fork (right) 🍴\n", get_current_time_ms() - data->start_time, philosopher_id + 1);
		pthread_mutex_unlock(&data->print);
		pthread_mutex_lock(&data->forks[left_fork]);
		pthread_mutex_lock(&data->print);
		pthread_mutex_lock(&data->death_mutex);
		if (data->philosopher_dead)
        {
            pthread_mutex_unlock(&data->death_mutex);
            return ; // Terminar el hilo si un filósofo ha muerto
        }
		pthread_mutex_unlock(&data->death_mutex);
		printf("🍴 %ld %d has taken a fork (left) 🍴\n", get_current_time_ms() - data->start_time, philosopher_id + 1);
		pthread_mutex_unlock(&data->print);
		pthread_mutex_lock(&data->death_mutex);
		if (data->philosopher_dead)
        {
            pthread_mutex_unlock(&data->death_mutex);
            return ; // Terminar el hilo si un filósofo ha muerto
        }
		pthread_mutex_unlock(&data->death_mutex);
		ft_eat(data, philosopher_id, left_fork, right_fork);
	}
}

void *routine(void *philo)
{
    t_filo *data;
    int     philosopher_id;
    int     left_fork;
    int     right_fork;

    data = (t_filo *)philo;
    pthread_mutex_lock(&data->id_mutex);
    philosopher_id = data->philosopher_id++;
    pthread_mutex_unlock(&data->id_mutex);
    left_fork = philosopher_id;
    right_fork = (philosopher_id + 1) % data->number_of_philosophers;

    if (data->number_of_philosophers % 2 == 0)
    {
        if ((philosopher_id + 1) % 2 == 0)
            my_usleep(100);
    }
    else
    {
        if ((philosopher_id + 1) % 2 != 0)
            my_usleep(100);
    }

    while (data->philosopher_dead == 0)
    {
        // Verificar si algún filósofo ha muerto
        pthread_mutex_lock(&data->death_mutex);
        if (data->philosopher_dead)
        {
            pthread_mutex_unlock(&data->death_mutex);
            return (NULL); // Terminar el hilo si un filósofo ha muerto
        }
        pthread_mutex_unlock(&data->death_mutex);

        data->eat[philosopher_id] = 0;
		pthread_mutex_lock(&data->death_mutex);
        if (data->philosopher_dead)
        {
            pthread_mutex_unlock(&data->death_mutex);
            return (NULL); // Terminar el hilo si un filósofo ha muerto
        }
        pthread_mutex_unlock(&data->death_mutex);
        ft_take_fork(data, philosopher_id, left_fork, right_fork);
        if (data->eat[philosopher_id] == 1)
		{
			pthread_mutex_lock(&data->death_mutex);
			if (data->philosopher_dead)
			{
				pthread_mutex_unlock(&data->death_mutex);
				return (NULL); // Terminar el hilo si un filósofo ha muerto
			}
        	pthread_mutex_unlock(&data->death_mutex);
            ft_sleep(data, philosopher_id);
		}
        pthread_mutex_lock(&data->death_mutex);
        if (data->philosopher_dead)
        {
            pthread_mutex_unlock(&data->death_mutex);
            return (NULL); // Terminar el hilo si un filósofo ha muerto
        }
        pthread_mutex_unlock(&data->death_mutex);
        ft_think(data, philosopher_id);
    }
    return (NULL);
}

int create_monitoring_thread(t_filo *philo)
{

	if (pthread_create(&philo->monitoring_thread, NULL, monitoring, (void *)philo) != 0)
	{
		printf("Error: Thread creation failed\n");
		return (1);
	}
	return (0);
}

int create_threads_philo(t_filo *philo)
{
	int i;

	i = -1;
	philo->philos = malloc(sizeof(pthread_t) * philo->number_of_philosophers);
	philo->start_time = 0;
	if (create_monitoring_thread(philo) == 1)
		return (1);
	philo->start_time = get_current_time_ms();
	while (++i < philo->number_of_philosophers)
	{
		if (pthread_create(&philo->philos[i], NULL, routine, (void *)philo) != 0)
		{
			printf("Error: Thread creation failed\n");
			return (1);
		}
	}
	i = -1;
	while (++i < philo->number_of_philosophers)
	{
		if (pthread_join(philo->philos[i], NULL) != 0)
		{
			perror("Failed to join philosopher thread");
			return (1);
		}
	}
	if (pthread_join(philo->monitoring_thread, NULL) != 0)
	{
		perror("Failed to join monitoring thread");
		return (1);
	}
	return (0);
}

t_filo *init_mutex(t_filo *philo)
{
    int i;

    i = 0;
    philo->forks = malloc(sizeof(pthread_mutex_t) * philo->number_of_philosophers);
    if (philo->forks == NULL)
    {
        printf("Error: Memory allocation failed\n");
        return (NULL);
    }
    while (i < philo->number_of_philosophers)
    {
        if (pthread_mutex_init(&philo->forks[i], NULL) != 0)
        {
            printf("Error: Mutex initialization failed\n");
            free(philo->forks);
            return (NULL);
        }
        i++;
    }
    if (pthread_mutex_init(&philo->print, NULL) != 0 ||
        pthread_mutex_init(&philo->id_mutex, NULL) != 0 ||
        pthread_mutex_init(&philo->meal_time_mutex, NULL) != 0 ||
        pthread_mutex_init(&philo->death_mutex, NULL) != 0) // Inicializar el death_mutex
    {
        printf("Error: Mutex initialization failed\n");
        free(philo->forks);
        return (NULL);
    }
    philo->philosopher_dead = 0; // Inicializar bandera de muerte en 0
    return (philo);
}

int check_error_init_argv(t_filo *philo)
{
	if (philo->number_of_philosophers == -2 || philo->time_to_die == -2
		|| philo->time_to_eat == -2 || philo->time_to_sleep == -2
		|| philo->number_of_times_each_philosopher_must_eat == -2)
	{
		return (1);
	}
	return (0);
}

t_filo *init_argv(char **argv)
{
	t_filo *data;
	int i;

	data = malloc(sizeof(t_filo));
	if (data == NULL)
	{
		printf("Error: Memory allocation failed\n");
		return (NULL);
	}
	data->number_of_philosophers = ft_atol(argv[1]);
	data->time_to_die = ft_atol(argv[2]);
	data->time_to_eat = ft_atol(argv[3]);
	data->time_to_sleep = ft_atol(argv[4]);
	if (argv[5])
		data->number_of_times_each_philosopher_must_eat = ft_atol(argv[5]);
	else
		data->number_of_times_each_philosopher_must_eat = -1;
	if (check_error_init_argv(data) == 1)
	{
		free(data);  // Liberar memoria si hay error
		return (NULL);
	}
	data->last_meal_time = malloc(sizeof(long) * data->number_of_philosophers);
	if (!data->last_meal_time)
	{
		perror("Malloc failed for last_meal_time");
		return(NULL);
	}
	i = -1;
	while ( ++i < data->number_of_philosophers)
		data->last_meal_time[i] = get_current_time_ms();
	data->eat = malloc(sizeof(int) * data->number_of_philosophers);
	if (!data->eat)
	{
		perror("Malloc failed for eat");
		return(NULL);
	}
	return (data);
}

void ft_free_error(t_filo *philo)
{
	if (philo)
	{
		free(philo->forks);
		free(philo->philos);
		free(philo);
	}
}

int main(int argc, char **argv)
{
	t_filo  *philo;
	int     error;
	
	if (argc < 5 || argc > 6)
	{
		printf("Error: Invalid number of arguments\n");
		return (1);
	}
	philo = init_argv(argv);
	if (philo == NULL)
		return (1);
	if (init_mutex(philo) == NULL)
	{
		ft_free_error(philo);
		return (1);
	}
	error = create_threads_philo(philo);
	if (error == 1)
	{
		ft_free_error(philo);
		return (1);
	}
	ft_free_error(philo);  // Liberar memoria antes de salir
	return (0);
}
