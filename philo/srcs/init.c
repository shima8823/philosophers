/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shima <shima@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/08 11:01:10 by shima             #+#    #+#             */
/*   Updated: 2022/09/10 11:17:16 by shima            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

static bool	args_to_variable(int argc, char *argv[], t_monitor *monitor);
static bool	malloc_variable(t_monitor *monitor);
static bool	mutex_init(t_monitor *monitor);

bool	init_philo(t_monitor *monitor)
{
	t_philo	*philos;
	int		i;

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
		philos[i].is_ate = false;
		philos[i].time_last_meal = 0;
		philos[i].monitor = monitor;
		if (pthread_mutex_init(&(philos[i].m_count_eat), NULL)
			|| pthread_mutex_init(&(philos[i].m_time_last_meal), NULL))
			return (false);
		i++;
	}
	return (true);
}

bool	init_monitor(int argc, char *argv[], t_monitor *monitor)
{
	monitor->is_must_eat = true;
	monitor->count_philos_ate = 0;
	if (!args_to_variable(argc, argv, monitor))
	{
		free(monitor);
		return (false);
	}
	if (!malloc_variable(monitor))
	{
		free(monitor);
		return (false);
	}
	if (!mutex_init(monitor))
	{
		all_free(monitor);
		return (false);
	}
	return (true);
}

static bool	args_to_variable(int argc, char *argv[], t_monitor *monitor)
{
	monitor->num_of_philos = ft_atoi(argv[1]);
	monitor->time_to_die = ft_atoi(argv[2]);
	monitor->time_to_eat = ft_atoi(argv[3]);
	monitor->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		monitor->times_philo_must_eat = ft_atoi(argv[5]);
	else
		monitor->is_must_eat = false;
	if (monitor->num_of_philos == -1
		|| monitor->time_to_die == -1
		|| monitor->time_to_eat == -1
		|| monitor->time_to_sleep == -1
		|| monitor->times_philo_must_eat == -1
		|| monitor->num_of_philos == 0)
		return (false);
	return (true);
}

static bool	malloc_variable(t_monitor *monitor)
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
	monitor->threads = malloc(sizeof(pthread_t) * monitor->num_of_philos);
	if (!monitor->threads)
	{
		free(monitor->forks);
		free(monitor->philos);
		return (false);
	}
	return (true);
}

static bool	mutex_init(t_monitor *monitor)
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
