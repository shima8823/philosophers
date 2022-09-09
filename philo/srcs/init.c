/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shima <shima@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/08 11:01:10 by shima             #+#    #+#             */
/*   Updated: 2022/09/09 11:57:02 by shima            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

static bool	args_to_variable(int argc, char *argv[], t_monitor *monitor);
static bool	malloc_variable(t_monitor *monitor);
static bool	mutex_init(t_monitor *monitor);

bool init_monitor(int argc, char *argv[], t_monitor *monitor)
{

	if (!args_to_variable(argc, argv, monitor))
		return (false);
	if (!malloc_variable(monitor))
		return (false);
	if (!mutex_init(monitor))
	{
		all_free(monitor);
		return (false);
	}
	monitor->count_philos_ate = 0;
	monitor->is_error = false;
	return (true);
}

static bool args_to_variable(int argc, char *argv[], t_monitor *monitor)
{
	monitor->num_of_philos = ft_atoi(argv[1]);
	monitor->time_to_die = ft_atoi(argv[2]);
	monitor->time_to_eat = ft_atoi(argv[3]);
	monitor->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		monitor->times_philo_must_eat = ft_atoi(argv[5]);
	else
		monitor->times_philo_must_eat = -2;
	if (monitor->num_of_philos == -1
		|| monitor->time_to_die == -1
		|| monitor->time_to_eat == -1
		|| monitor->time_to_sleep == -1
		|| monitor->times_philo_must_eat == -1
		|| monitor->num_of_philos == 0)
		return (false);
	return (true);
}

static bool malloc_variable(t_monitor *monitor)
{
	monitor->forks = malloc(sizeof(pthread_mutex_t) * monitor->num_of_philos);
	if (!monitor->forks)
		return (false);
	monitor->philos = malloc(sizeof(t_philo) * monitor->num_of_philos);
	if (!monitor->philos)
	{
		free(monitor->forks);
		return (false);
	}
	monitor->thread = malloc(sizeof(pthread_t) * monitor->num_of_philos);
	if (!monitor->thread)
	{
		free(monitor->forks);
		free(monitor->philos);
		return (false);
	}
	return (true);
}

static bool mutex_init(t_monitor *monitor)
{
	int	i;

	i = 0;
	while (i < monitor->num_of_philos)
	{
		if (pthread_mutex_init(&(monitor->forks[i]), NULL))
			return (false);
		i++;
	}
	if (pthread_mutex_init(&(monitor->m_is_finish), NULL)
		|| pthread_mutex_init(&(monitor->m_writing), NULL)
		|| pthread_mutex_init(&(monitor->m_count_philos_ate), NULL)
		|| pthread_mutex_lock(&(monitor->m_is_finish)))
		return (false);
	return (true);
}

bool init_philo(t_monitor *monitor)
{
	t_philo *philos;
	int	i;
	bool	is_valid;
	
	philos = monitor->philos;
	i = 0;
	while (i < monitor->num_of_philos)
	{
		philos[i].id = i + 1;
		philos[i].right = i;
		philos[i].left = i + 1;
		if (philos[i].left == monitor->num_of_philos)
			philos[i].left = 0;
		philos[i].count_eat = 0;
		is_valid = pthread_mutex_init(&(philos[i].m_count_eat), NULL);
		philos[i].is_ate = false;
		philos[i].time_last_meal = 0;
		is_valid = pthread_mutex_init(&(philos[i].m_time_last_meal), NULL);
		is_valid = pthread_mutex_init(&(philos[i].m_is_thinking), NULL);
		philos[i].monitor = monitor;
		i++;
	}
	return (!is_valid);
}
