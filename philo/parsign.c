/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsign.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apple <apple@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/03 18:43:50 by apple             #+#    #+#             */
/*   Updated: 2024/11/03 18:43:52 by apple            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long int	ft_atoi(char *str)
{
	int	res;

	res = 0;
	while ((*str > 9 && *str <= 13) || *str == 32)
		str++;
	if (*str == '-')
		return (-1);
	else if (*str == '+')
		str++;
	while (*str >= '0' && *str <= '9')
	{
		res *= 10;
		res += (*str - '0');
		str++;
	}
	if (*str != 0)
		return (-1);
	return (res);
}

int	check_argu(t_arguments *argu, char **argv)
{
	argu->number_of_philos = ft_atoi(argv[1]);
	argu->time_to_die = ft_atoi(argv[2]);
	argu->time_to_eat = ft_atoi(argv[3]);
	argu->time_to_sleep = ft_atoi(argv[4]);
	if (argv[5] != 0)
	{
		argu->max_meals = ft_atoi(argv[5]);
		if (argu->max_meals < 1)
			return (-1);
	}
	else
		argu->max_meals = -1;
	if (argu->number_of_philos < 1 || argu->time_to_die < 1
		|| argu->time_to_eat < 1 || argu->time_to_sleep < 1)
		return (-1);
	return (1);
}
