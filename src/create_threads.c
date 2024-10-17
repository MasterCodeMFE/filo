/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_threads.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manufern <manufern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 12:28:15 by manufern          #+#    #+#             */
/*   Updated: 2024/10/03 13:07:14 by manufern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../filo.h"

int	create_monitoring_thread(t_filo *philo)
{
	if (pthread_create(&philo->monitoring_thread,
			NULL, monitoring, (void *)philo) != 0)
	{
		printf("Error: Thread creation failed\n");
		return (1);
	}
	return (0);
}

int	create_threads_philo(t_filo *philo)
{
	int	i;

	i = -1;
	philo->philos = malloc(sizeof(pthread_t) * philo->number_philo);
	philo->start_time = 0;
	philo->start_time = get_current_time_ms();
	while (philo->philos && ++i < philo->number_philo)
	{
		if (philo->philos && pthread_create(&philo->philos[i],
				NULL, routine, (void *)philo) != 0)
			return (1);
	}
	if (create_monitoring_thread(philo) == 1 || !philo->philos)
		return (1);
	i = -1;
	while (++i < philo->number_philo)
	{
		if (pthread_join(philo->philos[i], NULL) != 0)
			return (1);
	}
	if (pthread_join(philo->monitoring_thread, NULL) != 0)
		return (1);
	return (0);
}
