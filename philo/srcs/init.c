/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shima <shima@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/08 11:01:10 by shima             #+#    #+#             */
/*   Updated: 2022/09/08 11:02:34 by shima            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

bool init_monitor(int argc, char *argv[], t_monitor *monitor)
{
	int i;
	bool is_valid;

	if (argc != 5 && argc != 6)
		return (false);
	is_valid = true;
	monitor->num_of_philos = ft_atoi(argv[1], &is_valid);
	monitor->time_to_die = ft_atoi(argv[2], &is_valid);
	monitor->time_to_eat = ft_atoi(argv[3], &is_valid);
	monitor->time_to_sleep = ft_atoi(argv[4], &is_valid);
	if (argc == 6)
		monitor->times_philo_must_eat = ft_atoi(argv[5], &is_valid);
	else
		monitor->times_philo_must_eat = -1;
	if (monitor->num_of_philos == 0 || !is_valid)
		return (false);
	monitor->count_philos_ate = 0;
	monitor->forks = malloc(sizeof(pthread_mutex_t) * monitor->num_of_philos);
	if (!monitor->forks)
		return (false);

	i = 0;
	while (i < monitor->num_of_philos)
	{
		pthread_mutex_init(&(monitor->forks[i]), NULL);
		i++;
	}
	pthread_mutex_init(&(monitor->m_is_finish), NULL);
	pthread_mutex_init(&(monitor->m_writing), NULL);
	pthread_mutex_init(&(monitor->m_count_philos_ate), NULL);
	pthread_mutex_lock(&(monitor->m_is_finish));
	return (true);
}

bool init_philo(t_monitor *monitor)
{
	t_philo *philos;
	int	i;
	
	philos = malloc(sizeof(t_philo) * monitor->num_of_philos);
	if (!(philos))
		return (false);
	monitor->philos = philos;
	i = 0;
	while (i < monitor->num_of_philos)
	{
		philos[i].id = i + 1;
		philos[i].right = i;
		philos[i].left = i + 1;
		if (philos[i].left == monitor->num_of_philos)
			philos[i].left = 0;
		philos[i].count_eat = 0;
		pthread_mutex_init(&(philos[i].m_count_eat), NULL);
		philos[i].is_ate = false;
		philos[i].time_last_meal = 0;
		pthread_mutex_init(&(philos[i].m_time_last_meal), NULL);
		philos[i].monitor = monitor;
		i++;
	}
	return (true);
}
