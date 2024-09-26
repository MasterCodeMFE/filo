/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manufern <manufern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 11:11:44 by manufern          #+#    #+#             */
/*   Updated: 2024/09/26 17:16:28 by manufern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../filo.h"

void *routine(void *philo)
{
    t_filo *data;
    int     number_of_philosopher;

    data = (t_filo *)philo;
    pthread_mutex_lock(&data->id_mutex);
	number_of_philosopher = data->philosopher_id++;
	pthread_mutex_unlock(&data->id_mutex);
    pthread_mutex_lock(&data->print);
    printf("id: %d\n", number_of_philosopher + 1);
    pthread_mutex_unlock(&data->print);
    return (NULL);
}

int create_threads_philo(t_filo **philo)
{
    t_filo *data;
    int i;

    data = *philo;
    i = 0;
    data->philos = malloc(sizeof(pthread_t) * data->number_of_philosophers);
    while(i < data->number_of_philosophers)
    {
        if (pthread_create(&data->philos[i], NULL, routine, (void *)data) != 0)
        {
            printf("Error: Thread creation failed\n");
            return (1);
        }
        i++;
    }
    i = 0;
    while (i < data->number_of_philosophers)
    {
        if (pthread_join(data->philos[i], NULL) != 0)
        {
            perror("Failed to join philosopher thread");
            exit(EXIT_FAILURE);
        }
        i++;
    }
    return (0);
}

t_filo *init_mutex(t_filo **philo)
{
    t_filo *data;
    int i;

    data = *philo;
    i = 0;
    data->forks = malloc(sizeof(pthread_mutex_t) * data->number_of_philosophers);
    if(data->forks == NULL)
    {
        printf("Error: Memory allocation failed\n");
        return (NULL);
    }
    while(i < data->number_of_philosophers)
    {
        if(pthread_mutex_init(&data->forks[i], NULL) == -1)
        {
            printf("Error: Mutex initialization failed\n");
            return (NULL);
        }
        i ++;
    }
    if(pthread_mutex_init(&data->print, NULL) == -1)
    {
        printf("Error: Mutex initialization failed\n");
        return (NULL);
    }
    if(pthread_mutex_init(&data->id_mutex, NULL) == -1)
    {
        printf("Error: Mutex initialization failed\n");
        return (NULL);
    }
    return (data); 
}

int check_error_init_argv(t_filo **philo)
{
    t_filo *data;

    data = *philo;
    if (data->number_of_philosophers == -2 || data->time_to_die == -2
        || data->time_to_eat == -2 || data->time_to_sleep == -2
        || data->number_of_times_each_philosopher_must_eat == -2)
    {
        return (1);
    }
    return (0);
}

t_filo *init_argv(char **argv)
{
    t_filo *data;

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
    if (check_error_init_argv(&data) == 1)
    {
        free(data);
        return (NULL);
    }
    return (data);
}

void    ft_free_error(t_filo *philo)
{
    free(philo);
    if (philo->forks)
        free(philo->forks);
    if(philo->philos)
        free(philo->philos);
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
    philo = init_mutex(&philo);
    if (philo == NULL)
    {
        ft_free_error(philo);
        return (1);
    }
    error = create_threads_philo(&philo);
    if (error == 1)
    {
        ft_free_error(philo);
        return (1);
    }
    return (0);
}
