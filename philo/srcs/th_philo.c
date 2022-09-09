/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   th_philo.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shima <shima@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/08 11:03:40 by shima             #+#    #+#             */
/*   Updated: 2022/09/08 14:02:27 by shima            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	*philosophers(void *arg)
{
	t_philo		*philo;
	t_monitor	*monitor;

	philo = arg;
	monitor = philo->monitor;
	if (philo->id % 2 == 1)
		usleep(800);
	while (true)
	{
		grab_forks(monitor, philo);
		eating(philo, monitor->time_to_eat);
		down_forks(monitor, philo->right, philo->left);
		sleeping(monitor->time_to_sleep, philo->id, monitor, philo);
		thinking(philo->id, monitor, &(philo->m_is_thinking), philo);
	}
	return (NULL);
}

void	grab_forks(t_monitor *monitor, t_philo *philo)
{
	pthread_mutex_lock(&(monitor->forks[philo->right]));
	print_log(philo->id, "has taken a fork", monitor, philo);
	pthread_mutex_lock(&(monitor->forks[philo->left]));
	print_log(philo->id, "has taken a fork", monitor, philo);
}

void	eating(t_philo *philo, int time_to_eat)
{
	pthread_mutex_lock(&(philo->m_time_last_meal));
	philo->time_last_meal = get_timestamp();
	pthread_mutex_unlock(&(philo->m_time_last_meal));
	print_log(philo->id, "is eating", philo->monitor, philo);
	while (get_timestamp() - philo->time_last_meal < time_to_eat)
		usleep(1000);
	pthread_mutex_lock(&(philo->m_count_eat));
	philo->count_eat++;
	pthread_mutex_unlock(&(philo->m_count_eat));
}

void	down_forks(t_monitor *monitor, int right, int left)
{
	pthread_mutex_unlock(&(monitor->forks[right]));
	pthread_mutex_unlock(&(monitor->forks[left]));
}

void	sleeping(int time_to_sleep, int id, t_monitor *monitor, t_philo *philo)
{
	long long	time_start;

	print_log(id, "is sleeping", monitor, philo);
	time_start = get_timestamp();
	time_start += time_to_sleep;
	while (get_timestamp() < time_start)
		usleep(1000);
}

void	thinking(int id, t_monitor *monitor, pthread_mutex_t *m_is_thinking, t_philo *philo)
{
	pthread_mutex_lock(m_is_thinking);
	print_log(id, "is thinking", monitor, philo);
	pthread_mutex_unlock(m_is_thinking);
}