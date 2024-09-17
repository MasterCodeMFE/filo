/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   filo.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manufern <manufern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 10:43:34 by manufern          #+#    #+#             */
/*   Updated: 2024/09/17 12:41:24 by manufern         ###   ########.fr       */
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
# include "./filo_struc.h"

int	ft_atoi(const char *str);
void *philosopher(void *arg);

#endif