/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   filo.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manufern <manufern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 10:43:34 by manufern          #+#    #+#             */
/*   Updated: 2024/09/23 16:52:20 by manufern         ###   ########.fr       */
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
# include <sys/time.h>
# include <stdint.h> 
# include "./filo_struc.h"

long	ft_atol(const char *str);
void *philosopher(void *arg);
void *philosopher(void *arg);
void my_usleep(unsigned int microseconds);
long get_current_time_ms();
void *monitor_philosophers(void *arg);

#endif