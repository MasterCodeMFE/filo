/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manufern <manufern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 11:11:44 by manufern          #+#    #+#             */
/*   Updated: 2024/10/03 13:12:13 by manufern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../filo.h"

int	check_error_init_argv(t_filo *philo)
{
	if (philo->number_philo == -2 || philo->time_to_die == -2
		|| philo->time_to_eat == -2 || philo->time_to_sleep == -2
		|| philo->number_of_times_eat == -2)
	{
		return (1);
	}
	return (0);
}

t_filo	*init_arrays(t_filo *data)
{
	int	i;

	data->last_meal_time = malloc(sizeof(long) * data->number_philo);
	if (!data->last_meal_time)
		return (perror("Malloc failed for last_meal_time"), NULL);
	data->number_of_eats = malloc(sizeof(int) * data->number_philo);
	if (!data->number_of_eats)
		return (perror("Malloc failed for number_of_eats"), NULL);
	i = -1;
	while (++i < data->number_philo)
	{
		data->last_meal_time[i] = get_current_time_ms();
		data->number_of_eats[i] = 0;
	}
	data->eat = malloc(sizeof(int) * data->number_philo);
	if (!data->eat)
		return (perror("Malloc failed for eat"), NULL);
	return (data);
}

t_filo	*init_argv(char **argv)
{
	t_filo	*data;

	data = malloc(sizeof(t_filo));
	if (data == NULL)
		return (printf("Error: Memory allocation failed\n"), NULL);
	data->number_philo = ft_atol(argv[1]);
	data->time_to_die = ft_atol(argv[2]);
	data->time_to_eat = ft_atol(argv[3]);
	data->time_to_sleep = ft_atol(argv[4]);
	data->number_of_times_eat = ft_atol(argv[5]);
	if (check_error_init_argv(data) == 1)
		return (free(data), NULL);
	if (!init_arrays(data))
		return (free(data), NULL);
	return (data);
}

void	ft_free_error(t_filo *philo)
{
	if (philo)
	{
		free(philo->last_meal_time);
		free(philo->number_of_eats);
		free(philo->eat);
		free(philo->forks);
		free(philo);
	}
}

int	main(int argc, char **argv)
{
	t_filo	*philo;
	int		error;

	if (argc < 5 || argc > 6)
	{
		printf("Error: Invalid number of arguments\n");
		return (1);
	}
	philo = init_argv(argv);
	if (philo == NULL)
		return (1);
	if (init_mutex(philo) == NULL)
	{
		ft_free_error(philo);
		return (1);
	}
	error = create_threads_philo(philo);
	if (error == 1)
	{
		ft_free_error(philo);
		return (1);
	}
	ft_free_error(philo);
	return (0);
}
