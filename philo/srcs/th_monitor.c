/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   th_monitor.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shima <shima@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/08 11:02:56 by shima             #+#    #+#             */
/*   Updated: 2022/09/09 13:04:44 by shima            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

static bool	is_philo_dead(t_philo *philo, long long timestamp);
static bool	is_philos_ate(t_philo *philo, t_monitor *monitor);

void	*monitor_routine(void *arg)
{
	t_philo		*philo;
	t_monitor	*monitor;
	bool		do_philo_must_eat;
	long long	timestamp;

	philo = arg;
	monitor = philo->monitor;
	do_philo_must_eat = false;
	if (monitor->times_philo_must_eat != -2)
		do_philo_must_eat = true;
	while (true)
	{
		timestamp = get_timestamp();
		if (is_philo_dead(philo, timestamp))
		{
			pthread_mutex_lock(&(monitor->m_writing));
			printf("%lld %d died\n", timestamp, philo->id);
			pthread_mutex_unlock(&(monitor->m_is_finish));
			return (NULL);
		}
		if (do_philo_must_eat && is_philos_ate(philo, monitor))
		{
			pthread_mutex_unlock(&(monitor->m_is_finish));
			return (NULL);
		}
		usleep(1000);
	}
	return (NULL);
}

static bool	is_philo_dead(t_philo *philo, long long timestamp)
{
	t_monitor	*monitor;

	monitor = philo->monitor;
	pthread_mutex_lock(&(philo->m_time_last_meal));
	if (philo->time_last_meal != 0)
	{
		if (timestamp - philo->time_last_meal >= monitor->time_to_die)
			return (true);
	}
	pthread_mutex_unlock(&(philo->m_time_last_meal));
	return (false);
}

static bool	is_philos_ate(t_philo *philo, t_monitor *monitor)
{
	pthread_mutex_lock(&(monitor->m_writing));
	pthread_mutex_lock(&(philo->m_count_eat));
	if (!(philo->is_ate) && philo->count_eat == monitor->times_philo_must_eat)
	{
		philo->is_ate = true;
		pthread_mutex_lock(&(monitor->m_count_philos_ate));
		monitor->count_philos_ate++;
		if (monitor->count_philos_ate == monitor->num_of_philos)
			return (true);
		pthread_mutex_unlock(&(monitor->m_count_philos_ate));
	}
	pthread_mutex_unlock(&(philo->m_count_eat));
	pthread_mutex_unlock(&(monitor->m_writing));
	return (false);
}
