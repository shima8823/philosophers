/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shima <shima@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/27 10:39:50 by shima             #+#    #+#             */
/*   Updated: 2022/09/06 13:47:24 by shima            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

// main
bool	init_monitor(int argc, char *argv[], t_monitor *monitor);
bool	init_philo(t_monitor *monitor);
void	*philosophers(void *arg);
long long	get_timestamp(void);
void	print_log(int id, char *act_msg, pthread_mutex_t *m_writing);


// philo
void	grab_fork(t_monitor *monitor, int id, int index);
void	eating(t_philo *philo, int time_to_eat);
void	down_forks(t_monitor *monitor, int right, int left);
void	sleeping(int time_to_sleep, int id, pthread_mutex_t *m_writing);
void	thinking(int id, pthread_mutex_t *m_writing);

// monitor
void	*monitor_thread(void *arg);
bool	is_philo_died(t_philo *philo);

// test
void	print_monitor(t_monitor monitor);

int	main(int argc, char *argv[])
{
	t_monitor	monitor;
	pthread_t	thread;
	int			i;

	if (!init_monitor(argc, argv, &monitor))
		return (1);
	init_philo(&monitor);
	// print_monitor(monitor);
	// printf("pointer m_writing: %p\n", &monitor.m_writing);
	// printf("pointer m_writing0: %p\n", &monitor.philos[0].monitor->m_writing);
	// printf("pointer m_writing2: %p\n", &monitor.philos[2].monitor->m_writing);
	// monitor.philos[0].monitor->time_to_die = 999;
	// print_monitor(monitor);
	
	i = 0;
	while (i < monitor.num_of_philos)
	{
		pthread_create(&monitor.philos[i].thread, NULL, philosophers, &(monitor.philos[i]));
		pthread_detach(monitor.philos[i].thread);
		i++;
	}
	i = 0;
	while (i < monitor.num_of_philos)
	{
		pthread_create(&thread, NULL, monitor_thread, &monitor.philos[i]);
		pthread_detach(thread);
		i++;
	}
	pthread_mutex_lock(&(monitor.m_is_finish));
	pthread_mutex_unlock(&(monitor.m_is_finish));
	return (0);
}

bool init_monitor(int argc, char *argv[], t_monitor *monitor)
{
	int i;
	if (argc != 5 && argc != 6)
		return (false);
	monitor->num_of_philos = ft_atoi(argv[1]);
	monitor->time_to_die = ft_atoi(argv[2]);
	monitor->time_to_eat = ft_atoi(argv[3]);
	monitor->time_to_sleep = ft_atoi(argv[4]);
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
		philos[i].time_last_meal = 0;
		pthread_mutex_init(&(philos[i].m_time_last_meal), NULL);
		philos[i].monitor = monitor;
		i++;
	}
	return (true);
}

void	*monitor_thread(void *arg)
{
	t_philo		*philo;
	t_monitor	*monitor;

	philo = arg;
	monitor = philo->monitor;
	while (true)
	{
		if (is_philo_died(philo))
		{
			// pthread_mutex_lock(&(monitor->m_writing));
			printf("%lld %d died\n", get_timestamp(), philo->id);
			pthread_mutex_unlock(&(monitor->m_is_finish));
			return (NULL);
		}
		else 
			pthread_mutex_unlock(&(monitor->m_writing));
	}
	return (NULL);
}

bool	is_philo_died(t_philo *philo)
{
	bool		ret;
	t_monitor	*monitor;

	ret = false;
	monitor = philo->monitor;
	pthread_mutex_lock(&(monitor->m_writing));
	pthread_mutex_lock(&(philo->m_time_last_meal));
	if (philo->time_last_meal != 0)
	{
		if (get_timestamp() - philo->time_last_meal >= monitor->time_to_die)
		{
			ret = true;
		}
	}
	pthread_mutex_unlock(&(philo->m_time_last_meal));
	return (ret);
}

void	*philosophers(void *arg)
{
	t_philo		*philo;
	t_monitor	*monitor;

	philo = arg;
	monitor = philo->monitor;
	if (philo->id % 2 == 1)
		usleep(200);
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
	pthread_mutex_lock(&(philo->m_time_last_meal));
	philo->time_last_meal = get_timestamp();
	pthread_mutex_unlock(&(philo->m_time_last_meal));
	print_log(philo->id, "is eating", &(philo->monitor->m_writing));
	while (get_timestamp() - philo->time_last_meal < time_to_eat)
		usleep(1 * 1000);
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

long long	get_timestamp(void)
{
	struct timeval	tv;
	if (gettimeofday(&tv, NULL) == -1)
		return (-1);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void	print_log(int id, char *act_msg, pthread_mutex_t *m_writing)
{
	pthread_mutex_lock(m_writing);
	printf("%lld %d %s\n", get_timestamp(), id, act_msg);
	pthread_mutex_unlock(m_writing);
}

// test

void	print_monitor(t_monitor monitor)
{
	int i;

	printf("num_of_philo: %d\n", monitor.num_of_philos);
	printf("time_to_die: %d\n", monitor.time_to_die);
	printf("time_to_eat: %d\n", monitor.time_to_eat);
	printf("time_to_sleep: %d\n", monitor.time_to_sleep);
	i = 0;
	while (i < monitor.num_of_philos)
	{
		printf("philo[%d].id: %d\n", i, monitor.philos[i].id);
		printf("philo[%d].right: %d\n", i, monitor.philos[i].right);
		printf("philo[%d].left: %d\n", i, monitor.philos[i].left);
		printf("---------------------------\n");
		i++;
	}
}