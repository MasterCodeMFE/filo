/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   filo.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manufern <manufern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 10:43:34 by manufern          #+#    #+#             */
/*   Updated: 2024/10/15 10:29:16 by manufern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FILO_H
# define FILO_H
# include <string.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <pthread.h>
# include <stdint.h> 
# include "./filo_struc.h"
# include <errno.h>

int					create_monitoring_thread(t_filo *philo);
int					create_threads_philo(t_filo *philo);
void				ft_eat(t_filo *philo, int philosopher_id,
						int left_fork, int right_fork);
long				ft_atol(char *str);
unsigned long long	get_time(void);
void				my_usleep(unsigned int microseconds);
t_filo				*init_mutex(t_filo *philo);
int					check_error_init_argv(t_filo *philo);
t_filo				*init_arrays(t_filo *data);
t_filo				*init_argv(char **argv);
void				ft_free_error(t_filo *philo);
void				ft_dead(t_filo *philo, int i, long current_time);
void				*monitoring_aux(t_filo *philo, int i,
						int *finished_philosophers,
						pthread_mutex_t *finished_mutex);
void				*monitoring(void *philo);
void				*ft_action(t_filo *philo, int philosopher_id,
						int left_fork, int right_fork);
void				*routine(void *philo);
void				ft_sleep(t_filo *philo, int philosopher_id);
void				take_fork_odd_aux(t_filo *philo,
						int philosopher_id, int left_fork, int right_fork);
void				take_fork_odd(t_filo *philo,
						int philosopher_id, int left_fork, int right_fork);
void				take_fork_even_aux(t_filo *philo,
						int philosopher_id, int left_fork, int right_fork);
void				take_fork_even(t_filo *philo,
						int philosopher_id, int left_fork, int right_fork);
void				ft_take_fork(t_filo *philo,
						int philosopher_id, int left_fork, int right_fork);
void				ft_think(t_filo *philo, int philosopher_id);
long				get_current_time_ms(void);
void				ft_print_sleep(t_filo *philo, int philosopher_id);

#endif