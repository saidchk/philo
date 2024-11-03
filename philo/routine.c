/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apple <apple@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/03 01:11:10 by apple             #+#    #+#             */
/*   Updated: 2024/11/03 01:23:24 by apple            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	go_to_sleep(t_philo_info *philo)
{
	display_state(philo, "is sleeping");
	ft_usleep(philo->time_to_sleep);
	display_state(philo, "is thinking");
}

void	go_to_eat(t_philo_info *philo)
{
	display_state(philo, "is eating");
	philo->last_meal = get_time();
	ft_usleep(philo->time_to_eat);
	philo->max_meals--;
	pthread_mutex_unlock(philo->right_fork);
	pthread_mutex_unlock(philo->left_fork);
	go_to_sleep(philo);
}

void	one_philo(t_philo_info *philo)
{
	display_state(philo, "has taken a fork");
	usleep(philo->time_to_die * 1000);
	printf("%lims %i is dead\n", get_time() - philo->start_time, philo->id);
	philo->max_meals = 0;
}
