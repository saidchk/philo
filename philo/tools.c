/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apple <apple@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 15:29:06 by apple             #+#    #+#             */
/*   Updated: 2024/11/11 16:36:03 by apple            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long int	get_time(void)
{
	struct timeval	curr_time;

	gettimeofday(&curr_time, NULL);
	return (curr_time.tv_sec * 1000 + (curr_time.tv_usec / 1000));
}

int	check_if_simulation_end(t_philo_info *philo)
{
	pthread_mutex_lock(philo->dead_lock);
	if (*philo->is_dead == true)
		return (pthread_mutex_unlock(philo->dead_lock), 1);
	pthread_mutex_unlock(philo->dead_lock);
	return (0);
}

void	ft_usleep(long int time, t_philo_info *philo)
{
	long int	start;

	start = get_time();
	while (time > (get_time() - start) && !check_if_simulation_end(philo))
	{
		usleep(50);
	}
}

void	display_state(t_philo_info *philo, char *state)
{
	struct timeval	curr_time;
	long int		time;

	pthread_mutex_lock(philo->writing_lock);
	gettimeofday(&curr_time, NULL);
	time = (curr_time.tv_sec * 1000 + (curr_time.tv_usec / 1000))
		- philo->start_time;
	if (check_if_simulation_end(philo))
	{
		pthread_mutex_unlock(philo->writing_lock);
		return ;
	}
	printf("%lims %i %s\n", time, philo->id, state);
	pthread_mutex_unlock(philo->writing_lock);
}

void	init_time(t_philo_info *philo, t_arguments arg, long int start)
{
	philo->start_time = start;
	philo->max_meals = arg.max_meals;
	philo->number_of_philos = arg.number_of_philos;
	philo->time_to_die = arg.time_to_die;
	philo->time_to_eat = arg.time_to_eat;
	philo->time_to_sleep = arg.time_to_sleep;
	philo->last_meal = start;
}
