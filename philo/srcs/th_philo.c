/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   th_philo.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shima <shima@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/08 11:03:40 by shima             #+#    #+#             */
/*   Updated: 2022/09/08 11:03:47 by shima            ###   ########.fr       */
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
		grab_fork(philo->monitor, philo->id, philo->right);
		grab_fork(philo->monitor, philo->id, philo->left);
		eating(philo, monitor->time_to_eat);
		down_forks(monitor, philo->right, philo->left);
		sleeping(monitor->time_to_sleep, philo->id, &(monitor->m_writing));
		thinking(philo->id, &(monitor->m_writing));
	}
	return (NULL);
}

void	grab_fork(t_monitor *monitor, int id, int index)
{
	pthread_mutex_lock(&(monitor->forks[index]));
	print_log(id, "has taken a fork", &(monitor->m_writing));
}

void	eating(t_philo *philo, int time_to_eat)
{
	print_log(philo->id, "is eating", &(philo->monitor->m_writing));
	pthread_mutex_lock(&(philo->m_time_last_meal));
	philo->time_last_meal = get_timestamp();
	pthread_mutex_unlock(&(philo->m_time_last_meal));
	while (get_timestamp() - philo->time_last_meal < time_to_eat)
		usleep(1 * 1000);
	pthread_mutex_lock(&(philo->m_count_eat));
	philo->count_eat++;
	pthread_mutex_unlock(&(philo->m_count_eat));
}

void	down_forks(t_monitor *monitor, int right, int left)
{
	pthread_mutex_unlock(&(monitor->forks[right]));
	pthread_mutex_unlock(&(monitor->forks[left]));
}

void	sleeping(int time_to_sleep, int id, pthread_mutex_t *m_writing)
{
	long long	time_start;

	time_start = get_timestamp();
	print_log(id, "is sleeping", m_writing);
	while (get_timestamp() - time_start < time_to_sleep)
		usleep(1 * 1000);
}

void	thinking(int id, pthread_mutex_t *m_writing)
{
	print_log(id, "is thinking", m_writing);
}