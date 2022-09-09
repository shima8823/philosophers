/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   th_philo.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shima <shima@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/08 11:03:40 by shima             #+#    #+#             */
/*   Updated: 2022/09/10 08:49:48 by shima            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

static void	grab_forks(t_monitor *monitor, t_philo *philo);
static void	eating(t_philo *philo, int time_to_eat);
static void	down_forks(t_monitor *monitor, int right, int left);
static void	sleeping(int time_to_sleep, int id, pthread_mutex_t *m_writing);

void	*philosopher_routine(void *arg)
{
	t_philo		*philo;
	t_monitor	*monitor;

	philo = arg;
	monitor = philo->monitor;
	if (philo->id % 2 == 1)
		usleep(500);
	while (true)
	{
		grab_forks(monitor, philo);
		eating(philo, monitor->time_to_eat);
		down_forks(monitor, philo->right, philo->left);
		sleeping(monitor->time_to_sleep, philo->id, &(monitor->m_writing));
		print_log(philo->id, "is thinking", &(monitor->m_writing));
	}
	return (NULL);
}

static void	grab_forks(t_monitor *monitor, t_philo *philo)
{
	pthread_mutex_lock(&(monitor->forks[philo->right]));
	print_log(philo->id, "has taken a fork", &(monitor->m_writing));
	pthread_mutex_lock(&(monitor->forks[philo->left]));
	print_log(philo->id, "has taken a fork", &(monitor->m_writing));
}

static void	eating(t_philo *philo, int time_to_eat)
{
	pthread_mutex_lock(&(philo->m_time_last_meal));
	philo->time_last_meal = get_timestamp();
	pthread_mutex_unlock(&(philo->m_time_last_meal));
	pthread_mutex_lock(&(philo->m_count_eat));
	philo->count_eat++;
	pthread_mutex_lock(&(philo->monitor->m_writing));
	pthread_mutex_lock(&(philo->monitor->m_count_philos_ate));
	if (philo->count_eat == philo->monitor->times_philo_must_eat
		&& philo->monitor->count_philos_ate == philo->monitor->num_of_philos - 1)
	{
		pthread_mutex_unlock(&(philo->monitor->m_count_philos_ate));
		pthread_mutex_unlock(&(philo->m_count_eat));
		return ;
	}
	printf("%lld %d %s\n", get_timestamp(), philo->id, "is eating");
	pthread_mutex_unlock(&(philo->monitor->m_writing));
	pthread_mutex_unlock(&(philo->monitor->m_count_philos_ate));
	pthread_mutex_unlock(&(philo->m_count_eat));
	// print_log(philo->id, "is eating", &(philo->monitor->m_writing));
	while (get_timestamp() - philo->time_last_meal < (long long)time_to_eat)
		usleep(1000);
}

static void	down_forks(t_monitor *monitor, int right, int left)
{
	pthread_mutex_unlock(&(monitor->forks[right]));
	pthread_mutex_unlock(&(monitor->forks[left]));
}

static void	sleeping(int time_to_sleep, int id, pthread_mutex_t *m_writing)
{
	long long	time_start;

	print_log(id, "is sleeping", m_writing);
	time_start = get_timestamp();
	while (get_timestamp() - time_start < (long long)time_to_sleep)
		usleep(1000);
}
