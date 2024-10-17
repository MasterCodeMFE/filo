/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manufern <manufern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/16 17:20:39 by manufern          #+#    #+#             */
/*   Updated: 2024/10/03 13:01:26 by manufern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../filo.h"

long	ft_atol(char *str)
{
	int		i;
	long	num;
	int		sig;

	i = 0;
	num = 0;
	sig = 1;
	if (str == NULL)
		return (-1);
	while (str[i] == ' ' || (str[i] >= '\t' && str[i] <= '\r'))
		i++;
	if (str[i] == '-')
		return (printf("Error: Número negativo no permitido\n"), -2);
	if (str[i] == '+' || str[i] == '-')
		i++;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (printf("Error: Entrada inválida\n"), -2);
		num = (num * 10) + (str[i] - '0');
		i++;
	}
	return (num * sig);
}
