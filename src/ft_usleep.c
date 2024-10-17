/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_usleep.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manufern <manufern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 09:56:34 by manufern          #+#    #+#             */
/*   Updated: 2024/10/15 10:20:17 by manufern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../filo.h"

unsigned long long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((unsigned long long)(tv.tv_sec * 1000000 + tv.tv_usec));
}

void	my_usleep(unsigned int microseconds)
{
	unsigned long long	start;

	start = get_time();
	while ((get_time() - start) < microseconds)
		usleep(microseconds / 1000);
}
