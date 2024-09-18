/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manufern <manufern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/16 17:20:39 by manufern          #+#    #+#             */
/*   Updated: 2024/09/18 10:43:46 by manufern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../filo.h"

long	ft_atol(const char *str)
{
	int		i;
	long	num;
	int		sig;

	i = 0;
	num = 0;
	sig = 1;
	if (str == NULL)
	{
		printf("Error: Cadena nula\n");
		exit(EXIT_FAILURE);
	}
	while (str[i] == ' ' || (str[i] >= '\t' && str[i] <= '\r'))
		i++;
	if (str[i] == '-')
	{
		printf("Error: Número negativo no permitido\n");
		exit(EXIT_FAILURE);
	}
	if (str[i] == '+' || str[i] == '-')
		i++;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
		{
			printf("Error: Entrada inválida\n");
			exit(EXIT_FAILURE);
		}
		num = (num * 10) + (str[i] - '0');
		i++;
	}
	return (num * sig);
}
