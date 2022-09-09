/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shima <shima@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/08 11:11:31 by shima             #+#    #+#             */
/*   Updated: 2022/09/09 20:42:31 by shima            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

static bool	create_threads(t_monitor *monitor);
static bool	philo_is_one(t_philo *philo, t_monitor *monitor);

int	main(int argc, char *argv[])
{
	t_monitor	monitor;

	if (argc != 5 && argc != 6)
		return (EXIT_FAILURE);
	if (!init_monitor(argc, argv, &monitor))
		return (EXIT_FAILURE);
	if (!init_philo(&monitor)
		|| !create_threads(&monitor))
	{
		all_free(&monitor);
		return (EXIT_FAILURE);
	}
	pthread_mutex_lock(&(monitor.m_is_finish));
	all_free(&monitor);
	pthread_mutex_unlock(&(monitor.m_is_finish));
	return (EXIT_SUCCESS);
}

static bool	create_threads(t_monitor *monitor)
{
	int	i;

	if (monitor->num_of_philos == 1)
		return (philo_is_one(&(monitor->philos[0]), monitor));
	i = 0;
	while (i < monitor->num_of_philos)
	{
		if (pthread_create(&monitor->philos[i].thread, NULL,
				philosopher_routine, &(monitor->philos[i]))
			|| pthread_detach(monitor->philos[i].thread))
			return (false);
		i++;
	}
	i = 0;
	while (i < monitor->num_of_philos)
	{
		if (pthread_create(&(monitor->threads[i]), NULL,
				monitor_routine, &monitor->philos[i])
			|| pthread_detach(monitor->threads[i]))
			return (false);
		i++;
	}
	return (true);
}

static bool	philo_is_one(t_philo *philo, t_monitor *monitor)
{
	long long	start_time;

	start_time = get_timestamp();
	printf("%lld %d has taken a fork\n", start_time, philo->id);
	while (get_timestamp() - start_time < (long long)(monitor->time_to_die))
		usleep(1000);
	printf("%lld %d died\n", get_timestamp(), philo->id);
	pthread_mutex_unlock(&(monitor->m_is_finish));
	return (true);
}
