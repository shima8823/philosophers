/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   th_monitor.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shima <shima@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/08 11:02:56 by shima             #+#    #+#             */
/*   Updated: 2022/09/08 11:03:07 by shima            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	*monitor_thread(void *arg)
{
	t_philo		*philo;
	t_monitor	*monitor;
	bool		do_philo_must_eat;

	philo = arg;
	monitor = philo->monitor;
	do_philo_must_eat = false;
	if (monitor->times_philo_must_eat != -1)
		do_philo_must_eat = true;
	while (true)
	{
		if (is_philo_dead(philo))
		{
			pthread_mutex_lock(&(monitor->m_writing));
			printf("%lld %d died\n", get_timestamp(), philo->id);
			pthread_mutex_unlock(&(monitor->m_is_finish));
			return (NULL);
		}
		// else 
			// pthread_mutex_unlock(&(monitor->m_writing));
		if (do_philo_must_eat && is_philos_ate(philo, monitor))
		{
			pthread_mutex_unlock(&(monitor->m_is_finish));
			break ;
		}
		// printf("monitor.count_eat: %d\n", monitor->count_philos_ate);
		// if (philo->count_eat == 2)
		// 	printf("\nid: [%d] ear_count: %d\n", philo->id, philo->count_eat);
		usleep(1000 * 3);
	}
	return (NULL);
}

bool	is_philo_dead(t_philo *philo)
{
	t_monitor	*monitor;

	monitor = philo->monitor;
	// pthread_mutex_lock(&(monitor->m_writing));
	pthread_mutex_lock(&(philo->m_time_last_meal));
	if (philo->time_last_meal != 0)
	{
		if (get_timestamp() - philo->time_last_meal >= monitor->time_to_die)
			return (true);
	}
	pthread_mutex_unlock(&(philo->m_time_last_meal));
	return (false);
}

bool	is_philos_ate(t_philo *philo, t_monitor *monitor)
{
	// printf("AAAm_writing_p: %p\n", &monitor->m_writing);
	pthread_mutex_lock(&(monitor->m_writing));
	pthread_mutex_lock(&(philo->m_count_eat));
	if (!(philo->is_ate) && philo->count_eat == monitor->times_philo_must_eat)
	{
		philo->is_ate = true;
		pthread_mutex_lock(&(monitor->m_count_philos_ate));
		monitor->count_philos_ate++;
		// printf("id: %d\n", philo->id);
		// printf("monitor->count_philos_ate: %d\n", monitor->count_philos_ate);
		if (monitor->count_philos_ate == monitor->num_of_philos)
		{
			// printf("id: %d\n", philo->id);
			// printf("count_eat: %d\n", philo->count_eat);
			// printf("monitor->count_philos_ate: %d\n", monitor->count_philos_ate);
			pthread_mutex_unlock(&(monitor->m_is_finish));
			return (true);
		}
		pthread_mutex_unlock(&(monitor->m_count_philos_ate));
	}
	pthread_mutex_unlock(&(philo->m_count_eat));
	pthread_mutex_unlock(&(monitor->m_writing));
	return (false);
}
