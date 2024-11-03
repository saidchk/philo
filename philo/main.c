/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apple <apple@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 15:43:08 by apple             #+#    #+#             */
/*   Updated: 2024/11/03 19:43:39 by apple            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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

void	if_full(int *counter, t_philo_info *philo)
{
	*counter++;
	philo->last_meal = -1;
}

void	is_dead(t_philo_info *data_philos)
{
	int			i;
	long int	curr;
	int			counter;

	i = 0;
	counter = 0;
	while (counter != data_philos[0].number_of_philos)
	{
		if (i == data_philos[0].number_of_philos)
			i = 0;
		curr = get_time() - data_philos[i].last_meal;
		if (data_philos[i].max_meals == 0 && data_philos[i].last_meal != -1)
			if_full(&counter, &data_philos[i]);
		else if (data_philos[i].last_meal > 0
			&& curr > data_philos[i].time_to_die)
		{
			pthread_mutex_lock(data_philos[i].dead_lock);
			*data_philos[i].is_dead = true;
			pthread_mutex_unlock(data_philos[i].dead_lock);
			display_state(&data_philos[i], "died");
			break ;
		}
		usleep(60);
		i++;
	}
}

void	creat_threads(t_data *philo_data)
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
	is_dead(philo_data->philos);
	i = 0;
	while (i < number_of_philos)
	{
		pthread_join(philo_data->philos[i].thread, NULL);
		i++;
	}
}

void	init(t_data *data_philos, pthread_mutex_t *forks, t_arguments arg)
{
	int			i;
	long int	start;

	i = 0;
	data_philos->is_dead = false;
	start = get_time();
	pthread_mutex_init(&data_philos->dead_lock, NULL);
	while (i < arg.number_of_philos)
	{
		pthread_mutex_init(&data_philos->philos[i].meal_lock, NULL);
		pthread_mutex_init(&forks[i], NULL);
		pthread_mutex_init(&data_philos->philos[i].writing_lock, NULL);
		init_time(&data_philos->philos[i], arg, start);
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
		i++;
	}
	return (0);
}
