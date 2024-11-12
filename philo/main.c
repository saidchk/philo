/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schakkou <schakkou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 15:43:08 by apple             #+#    #+#             */
/*   Updated: 2024/11/11 21:09:33 by schakkou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	if_full(int *counter, t_philo_info *philo)
{
	pthread_mutex_lock(&philo->last_meal_lock);
	if (philo->last_meal == -1)
	{
		pthread_mutex_unlock(&philo->last_meal_lock);
		return (0);
	}
	if (philo->max_meals == 0)
	{
		(*counter)++;
		philo->last_meal = -1;
		pthread_mutex_unlock(&philo->last_meal_lock);
		return (1);
	}
	pthread_mutex_unlock(&philo->last_meal_lock);
	return (0);
}

static void	is_died(t_philo_info *philos)
{
	int			i;
	long int	curr;
	int			counter;

	i = 0;
	counter = 0;
	while (counter != philos[0].number_of_philos)
	{
		i = i % 5;
		pthread_mutex_lock(&philos[i].last_meal_lock);
		curr = get_time() - philos[i].last_meal;
		pthread_mutex_unlock(&philos[i].last_meal_lock);
		if (if_full(&counter, &philos[i]))
			continue ;
		else if (philos[i].last_meal > 0 && curr > philos[i].time_to_die)
		{
			pthread_mutex_lock(philos[i].dead_lock);
			*philos[i].is_dead = true;
			pthread_mutex_unlock(philos[i].dead_lock);
			printf("%li %i is died\n", get_time() - philos[i].start_time, i + 1);
			break ;
		}
		i++;
	}
}

static void	creat_threads(t_data *philo_data)
{
	int	number_of_philos;
	int	i;

	i = 0;
	number_of_philos = philo_data->philos[0].number_of_philos;
	while (i < number_of_philos)
	{
		pthread_create(&philo_data->philos[i].thread, NULL, routine,
			&philo_data->philos[i]);
		i++;
	}
	is_died(philo_data->philos);
	i = 0;
	while (i < number_of_philos)
	{
		pthread_join(philo_data->philos[i].thread, NULL);
		i++;
	}
}

static void	init(t_data *data_philos, pthread_mutex_t *forks, t_arguments arg)
{
	int			i;
	long int	start;

	i = 0;
	data_philos->is_dead = false;
	start = get_time();
	pthread_mutex_init(&data_philos->dead_lock, NULL);
	pthread_mutex_init(&data_philos->write_lock, NULL);
	while (i < arg.number_of_philos)
	{
		pthread_mutex_init(&forks[i], NULL);
		//pthread_mutex_init(&data_philos->philos[i].meal_lock, NULL);
		pthread_mutex_init(&data_philos->philos[i].last_meal_lock, NULL);
		init_time(&data_philos->philos[i], arg, start);
		data_philos->philos[i].writing_lock = &data_philos->write_lock;
		data_philos->philos[i].dead_lock = &data_philos->dead_lock;
		data_philos->philos[i].right_fork = &forks[i];
		data_philos->philos[i].left_fork = &forks[i + 1];
		data_philos->philos[i].is_dead = &data_philos->is_dead;
		data_philos->philos[i].id = i + 1;
		i++;
	}
	data_philos->philos[i - 1].left_fork = &forks[0];
}

int	main(int ac, char **av)
{
	t_arguments		args;
	t_data			philo_data;
	t_philo_info	philos[200];
	int				i;
	pthread_mutex_t	forks[200];

	if (ac < 5 || ac > 6 || check_argu(&args, av) == -1)
	{
		write(1, "Error , Not valide arguments\n", 29);
		return (1);
	}
	i = 0;
	philo_data.philos = philos;
	init(&philo_data, forks, args);
	creat_threads(&philo_data);
	while (i < philo_data.philos[0].number_of_philos)
	{
		pthread_mutex_destroy(&forks[i]);
		//pthread_mutex_destroy(&philos[i].meal_lock);
		pthread_mutex_destroy(&philos[i].last_meal_lock);
		i++;
	}
	pthread_mutex_destroy(&philo_data.dead_lock);
	pthread_mutex_destroy(&philo_data.write_lock);
	return (0);
}
