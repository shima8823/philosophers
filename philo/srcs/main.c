/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shima <shima@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/27 10:39:50 by shima             #+#    #+#             */
/*   Updated: 2022/09/05 11:46:18 by shima            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

bool	init_monitor(int argc, char *argv[], t_monitor *monitor);
bool	init_philo(t_monitor *monitor);
void	*philosophers(void *arg);
long long	get_timestamp(void);

void	grab_fork(t_monitor *monitor, int id, int index);
void	eating(int id, long long *time_last_meal, int time_to_eat);
void	down_forks(t_monitor *monitor, int right, int left);
void	sleeping(int time_to_sleep, int id);
void	thinking(int id);


// test
void	print_monitor(t_monitor monitor);

int	main(int argc, char *argv[])
{
	t_monitor	monitor;
	int			i;

	
	if (!init_monitor(argc, argv, &monitor))
		return (1);
	init_philo(&monitor);
	print_monitor(monitor);
	
	i = 0;
	while (i < monitor.num_of_philos)
	{
		pthread_create(&monitor.philos[i].thread, NULL, philosophers, &(monitor.philos[i]));
		i++;
	}

	i = 0;
	while (i < monitor.num_of_philos)
	{
		pthread_join(monitor.philos[i].thread, NULL);
		i++;
	}

	return (0);
}

bool init_monitor(int argc, char *argv[], t_monitor *monitor)
{
	int i;
	if (argc != 5 && argc != 6)
		return (false);
	// printf("%s, %d\n", __FILE__, __LINE__);
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
	return (true);
}

bool init_philo(t_monitor *monitor)
{
	int	i;
	
	monitor->philos = malloc(sizeof(t_philo) * monitor->num_of_philos);
	if (!(monitor->philos))
		return (false);
	i = 0;
	while (i < monitor->num_of_philos)
	{
		monitor->philos[i].id = i + 1;
		monitor->philos[i].right = i;
		monitor->philos[i].left = i + 1;
		if (monitor->philos[i].left == monitor->num_of_philos)
			monitor->philos[i].left = 0;
		monitor->philos[i].monitor = monitor;
		i++;
	}
	return (true);
}

void	*philosophers(void *arg)
{
	t_philo *philo;
	t_monitor *monitor;

	philo = arg;
	monitor = philo->monitor;
	if (philo->id % 2 == 1)
		usleep(200);
	grab_fork(philo->monitor, philo->id, philo->right);
	grab_fork(philo->monitor, philo->id, philo->left);
// eating
	eating(philo->id, &(philo->time_last_meal), monitor->time_to_eat);
// down fork
	down_forks(monitor, philo->right, philo->left);
// sleep
	sleeping(monitor->time_to_sleep, philo->id);
// thinking
	thinking(philo->id);


	return (NULL);
}

void	grab_fork(t_monitor *monitor, int id, int index)
{
	pthread_mutex_lock(&(monitor->forks[index]));
	printf("%lld %d has taken a fork\n", get_timestamp(), id);
}

void	eating(int id, long long *time_last_meal, int time_to_eat)
{
	*time_last_meal = get_timestamp();
	printf("%lld %d is eating\n", *time_last_meal, id);
	while (get_timestamp() - *time_last_meal < time_to_eat)
		usleep(1 * 1000);
}

void	down_forks(t_monitor *monitor, int right, int left)
{
	pthread_mutex_unlock(&(monitor->forks[right]));
	pthread_mutex_unlock(&(monitor->forks[left]));
}

void	sleeping(int time_to_sleep, int id)
{
	long long	time_start;

	time_start = get_timestamp();
	printf("%lld %d is sleeping\n", time_start, id);
	while (get_timestamp() - time_start < time_to_sleep)
		usleep(1 * 1000);
}

void	thinking(int id)
{
	printf("%lld %d is is thinking\n", get_timestamp(), id);
}

long long	get_timestamp(void)
{
	struct timeval	tv;
	if (gettimeofday(&tv, NULL) == -1)
		return (-1);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
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