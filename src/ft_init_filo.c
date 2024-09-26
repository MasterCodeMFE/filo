/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_init_filo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manufern <manufern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 10:56:57 by manufern          #+#    #+#             */
/*   Updated: 2024/09/25 11:24:02 by manufern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../filo.h"

/* void allocate_filo_memory(t_filo **filo, char **argv)
{
    (*filo) = malloc(sizeof(t_filo));
    if (!*filo)
    {
        perror("Malloc failed for t_filo");
        exit(EXIT_FAILURE);
    }
    (*filo)->number_of_philosophers = ft_atol(argv[1]);
    (*filo)->time_to_die = ft_atol(argv[2]);
    (*filo)->time_to_eat = ft_atol(argv[3]);
    (*filo)->time_to_sleep = ft_atol(argv[4]);
    if (argv[5])
        (*filo)->number_of_times_each_philosopher_must_eat = ft_atol(argv[5]);
    else
        (*filo)->number_of_times_each_philosopher_must_eat = -1;
}

void allocate_filo_resources(t_filo **filo)
{
    (*filo)->forks = malloc(sizeof(pthread_mutex_t) * (*filo)->number_of_philosophers);
    (*filo)->last_meal_mutex = malloc(sizeof(pthread_mutex_t) * (*filo)->number_of_philosophers);
    (*filo)->laps = malloc(sizeof(int) * (*filo)->number_of_philosophers);
    (*filo)->eat = malloc(sizeof(int) * (*filo)->number_of_philosophers);
    (*filo)->laps_mutex = malloc(sizeof(pthread_mutex_t) * (*filo)->number_of_philosophers);
    (*filo)->pick_fork_mutex = malloc(sizeof(pthread_mutex_t) * (*filo)->number_of_philosophers);
    if (!(*filo)->forks || !(*filo)->last_meal_mutex || !(*filo)->laps || !(*filo)->eat || !(*filo)->laps_mutex)
    {
        perror("Malloc failed for forks or last_meal_mutex");
        exit(EXIT_FAILURE);
    }
    (*filo)->last_meal_time = malloc(sizeof(long) * (*filo)->number_of_philosophers);
    if (!(*filo)->last_meal_time)
    {
        perror("Malloc failed for last_meal_time");
        exit(EXIT_FAILURE);
    }
}

void initialize_mutexes(t_filo **filo)
{
    int i = 0;

    while (i < (*filo)->number_of_philosophers)
    {
        if (pthread_mutex_init(&(*filo)->forks[i], NULL) != 0
            || pthread_mutex_init(&(*filo)->last_meal_mutex[i], NULL) != 0
            || pthread_mutex_init(&(*filo)->laps_mutex[i], NULL) != 0
            || pthread_mutex_init(&(*filo)->pick_fork_mutex[i], NULL) != 0)
        {
            perror("Mutex init failed");
            exit(EXIT_FAILURE);
        }
        (*filo)->last_meal_time[i] = get_current_time_ms();
        i++;
    }
}

void finalize_filo_setup(t_filo **filo)
{
    if (pthread_mutex_init(&(*filo)->print, NULL) != 0)
    {
        perror("Mutex init failed");
        exit(EXIT_FAILURE);
    }
    (*filo)->init_program = get_current_time_ms();
    create_thread_philos(*filo);
    cleanup(*filo);
}

void init_filo_struct(t_filo **filo, char **argv)
{
    allocate_filo_memory(filo, argv);
    allocate_filo_resources(filo);
    initialize_mutexes(filo);
    (*filo)->init_program = get_current_time_ms();
    finalize_filo_setup(filo);
} */