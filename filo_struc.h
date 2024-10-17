/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   filo_struc.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manufern <manufern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 13:29:07 by manufern          #+#    #+#             */
/*   Updated: 2024/10/03 14:27:07 by manufern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FILO_STRUC_H
# define FILO_STRUC_H

typedef struct s_filo
{
	int				*laps;
	int				philosopher_id;
	int				philosopher_dead;
	int				*number_of_eats;
	int				*eat;
	long			number_philo;
	long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
	long			number_of_times_eat;
	long			start_time;
	long			*last_meal_time;
	pthread_t		monitoring_thread;
	pthread_t		*philos;
	pthread_mutex_t	print;
	pthread_mutex_t	id_mutex;
	pthread_mutex_t	meal_time_mutex;
	pthread_mutex_t	death_mutex;
	pthread_mutex_t	*forks;
}	t_filo;

#endif