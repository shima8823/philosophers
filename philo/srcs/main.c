/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shima <shima@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/27 10:39:50 by shima             #+#    #+#             */
/*   Updated: 2022/09/07 12:03:26 by shima            ###   ########.fr       */
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
bool	is_philo_dead(t_philo *philo);
bool	is_philos_ate(t_philo *philo, t_monitor *monitor);

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

long long	get_timestamp(void)
{
	struct timeval	tv;
	if (gettimeofday(&tv, NULL) == -1)
		return (-1);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void	print_log(int id, char *act_msg, pthread_mutex_t *m_writing)
{
	// printf("m_writing_p: %p\n", m_writing);
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