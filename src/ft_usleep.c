/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_usleep.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manufern <manufern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 09:56:34 by manufern          #+#    #+#             */
/*   Updated: 2024/09/25 11:32:27 by manufern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../filo.h"

u_int64_t get_time()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (u_int64_t)(tv.tv_sec * 1000000 + tv.tv_usec); // Devuelve el tiempo en microsegundos
}

void my_usleep(unsigned int microseconds)
{
    u_int64_t start = get_time();
    
    while ((get_time() - start) < microseconds) {
        usleep(microseconds / 100); // Duerme una décima parte del tiempo total
    }
}

