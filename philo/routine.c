/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apple <apple@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/03 01:11:10 by apple             #+#    #+#             */
/*   Updated: 2024/11/11 17:01:52 by apple            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	go_to_sleep(t_philo_info *philo)
{
	display_state(philo, "is sleeping");
	ft_usleep(philo->time_to_sleep, philo);
	display_state(philo, "is thinking");
}

void	go_to_eat(t_philo_info *philo)
{
	display_state(philo, "is eating");
	pthread_mutex_lock(&philo->last_meal_lock);
	philo->last_meal = get_time();
	pthread_mutex_unlock(&philo->last_meal_lock);
	ft_usleep(philo->time_to_eat, philo);
	pthread_mutex_lock(&philo->meal_lock);
	philo->max_meals--;
	pthread_mutex_unlock(&philo->meal_lock);
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

void	*routine(void *ptr)
{
	t_philo_info	*philo;

	philo = (t_philo_info *)(ptr);
	if (philo->number_of_philos == 1)
		return (one_philo(philo), NULL);
	while (!check_if_simulation_end(philo) && philo->max_meals != 0)
	{
		if (philo->id % 2 == 0)
		{
			pthread_mutex_lock(philo->right_fork);
			display_state(philo, "has taken a fork");
			pthread_mutex_lock(philo->left_fork);
			display_state(philo, "has taken a fork");
			go_to_eat(philo);
		}
		else
		{
			pthread_mutex_lock(philo->left_fork);
			display_state(philo, "has taken a fork");
			pthread_mutex_lock(philo->right_fork);
			display_state(philo, "has taken a fork");
			go_to_eat(philo);
		}
	}
	return (NULL);
}
