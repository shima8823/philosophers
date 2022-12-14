/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   th_philo.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shima <shima@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/08 11:03:40 by shima             #+#    #+#             */
/*   Updated: 2022/09/11 10:31:30 by shima            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

static void	grab_forks(t_monitor *monitor, int id, int right, int left);
static void	eating(t_philo *philo, int time_to_eat);
static void	down_forks(pthread_mutex_t *forks, int right, int left);
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
		grab_forks(monitor, philo->id, philo->right, philo->left);
		eating(philo, monitor->time_to_eat);
		down_forks(monitor->forks, philo->right, philo->left);
		sleeping(monitor->time_to_sleep, philo->id, &(monitor->m_writing));
		print_log(philo->id, "is thinking", &(monitor->m_writing));
	}
	return (NULL);
}

static void	grab_forks(t_monitor *monitor, int id, int right, int left)
{
	pthread_mutex_lock(&(monitor->forks[right]));
	print_log(id, "has taken a fork", &(monitor->m_writing));
	pthread_mutex_lock(&(monitor->forks[left]));
	print_log(id, "has taken a fork", &(monitor->m_writing));
}

static void	eating(t_philo *philo, int time_to_eat)
{
	t_monitor	*monitor;

	monitor = philo->monitor;
	pthread_mutex_lock(&(philo->m_time_last_meal));
	philo->time_last_meal = get_timestamp();
	pthread_mutex_unlock(&(philo->m_time_last_meal));
	pthread_mutex_lock(&(philo->m_count_eat));
	philo->count_eat++;
	pthread_mutex_lock(&(monitor->m_writing));
	pthread_mutex_lock(&(monitor->m_count_philos_ate));
	if (philo->count_eat != monitor->times_philo_must_eat
		|| monitor->count_philos_ate != monitor->num_of_philos - 1)
	{
		printf("%lld %d %s\n", get_timestamp(), philo->id, "is eating");
		pthread_mutex_unlock(&(monitor->m_writing));
	}
	pthread_mutex_unlock(&(monitor->m_count_philos_ate));
	pthread_mutex_unlock(&(philo->m_count_eat));
	while (get_timestamp() - philo->time_last_meal < (long long)time_to_eat)
		usleep(1000);
}

static void	down_forks(pthread_mutex_t *forks, int right, int left)
{
	pthread_mutex_unlock(&(forks[right]));
	pthread_mutex_unlock(&(forks[left]));
}

static void	sleeping(int time_to_sleep, int id, pthread_mutex_t *m_writing)
{
	long long	time_start;

	print_log(id, "is sleeping", m_writing);
	time_start = get_timestamp();
	while (get_timestamp() - time_start < (long long)time_to_sleep)
		usleep(1000);
}
