/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manufern <manufern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 09:49:51 by manufern          #+#    #+#             */
/*   Updated: 2024/10/03 13:07:14 by manufern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../filo.h"

void cleanup(t_filo *filo)
{
    int i;

    i = 0;
    while (i < filo->number_philo)
    {
        pthread_mutex_destroy(&filo->forks[i]);
        pthread_mutex_destroy(&filo->last_meal_mutex[i]);
        pthread_mutex_destroy(&filo->laps_mutex[i]);
        i++;
    }
    pthread_mutex_destroy(&filo->print);
    pthread_mutex_destroy(&filo->dead_mutex);
    pthread_mutex_destroy(&filo->id_mutex);
    free(filo->forks);
    free(filo->last_meal_mutex);
    free(filo->last_meal_time);
    free(filo->laps);
    free(filo->eat);
    free(filo->philos);
    free(filo->laps_mutex);
    free(filo);
}
