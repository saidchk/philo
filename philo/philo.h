/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schakkou <schakkou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 15:42:55 by apple             #+#    #+#             */
/*   Updated: 2024/11/11 19:02:42 by schakkou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdbool.h>
# include <stdio.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct arg
{
	int				number_of_philos;
	long int		max_meals;
	long int		time_to_die;
	long int		time_to_eat;
	long int		time_to_sleep;
}					t_arguments;

typedef struct philo_info
{
	int				id;
	int				number_of_philos;
	long int		time_to_die;
	long int		time_to_eat;
	long int		time_to_sleep;
	long int		max_meals;
	long int		last_meal;
	int				*is_dead;
	long int		start_time;
	pthread_mutex_t	meal_lock;
	pthread_mutex_t	*writing_lock;
	pthread_mutex_t	*dead_lock;
	pthread_mutex_t	*right_fork;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	last_meal_lock;
	pthread_t		thread;
}					t_philo_info;

typedef struct s_program
{
	int				is_dead;
	pthread_mutex_t	dead_lock;
	pthread_mutex_t	write_lock;
	t_philo_info	*philos;
}					t_data;

long int			get_time(void);
void				go_to_eat(t_philo_info *philo);
int					check_argu(t_arguments *argu, char **argv);
void				ft_usleep(long int time, t_philo_info *philo);
void				display_state(t_philo_info *philo, char *state);
int					check_if_simulation_end(t_philo_info *philo);
void				one_philo(t_philo_info *philo);
void				go_to_sleep(t_philo_info *philo);
void				*routine(void *ptr);
void				init_time(t_philo_info *philo, t_arguments arg,
						long int start);

#endif