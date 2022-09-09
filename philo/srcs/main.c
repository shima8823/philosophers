/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shima <shima@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/08 11:11:31 by shima             #+#    #+#             */
/*   Updated: 2022/09/09 12:02:29 by shima            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

static bool	create_philo_thread(t_monitor *monitor);
static bool	create_monitor_thread(t_monitor *monitor);

// test
void	print_monitor(t_monitor monitor);

int	main(int argc, char *argv[])
{
	t_monitor	monitor;

	if (argc != 5 && argc != 6)
		return (EXIT_FAILURE);
	if (!init_monitor(argc, argv, &monitor))
		return (1);
	if (!init_philo(&monitor))
		return (1);
	// print_monitor(monitor);
	// printf("pointer m_writing: %p\n", &monitor.m_writing);
	// printf("pointer m_writing0: %p\n", &monitor.philos[0].monitor->m_writing);
	// printf("pointer m_writing2: %p\n", &monitor.philos[2].monitor->m_writing);
	// monitor.philos[0].monitor->time_to_die = 999;
	// print_monitor(monitor);
	if (!create_monitor_thread(&monitor))
		return (1);
	if (!create_philo_thread(&monitor))
		return (1);
	pthread_mutex_lock(&(monitor.m_is_finish));
	all_free(&monitor);
	pthread_mutex_unlock(&(monitor.m_is_finish));
	return (0);
}

static bool	create_philo_thread(t_monitor *monitor)
{
	int	i;

	i = 0;
	while (i < monitor->num_of_philos)
	{
		if (pthread_create(&monitor->philos[i].thread, NULL, philosophers, &(monitor->philos[i])))
			return (false);
		if (pthread_detach(monitor->philos[i].thread))
			return (false);
		i++;
	}
	return (true);
}

static bool	create_monitor_thread(t_monitor *monitor)
{
	pthread_t	*thread;
	int			i;

	thread = malloc(sizeof(pthread_t) * monitor->num_of_philos);
	if (!thread)
		return (false);
	i = 0;
	while (i < monitor->num_of_philos)
	{
		if (pthread_create(&(thread[i]), NULL, monitor_thread, &monitor->philos[i]))
		{
			free(thread);
			return (false);
		}
		if (pthread_detach(thread[i]))
		{
			free(thread);
			return (false);
		}
		i++;
	}
	return (true);
}

void	print_log(int id, char *act_msg, t_monitor *monitor, t_philo *philo)
{
	long long curretn_time;
	long long past_time;
	// printf("m_writing			: %p\n", &(monitor->m_writing));
	curretn_time = get_timestamp();
	pthread_mutex_lock(&(monitor->m_writing));
	printf("%lld %d %s\n", get_timestamp(), id, act_msg);
	pthread_mutex_unlock(&(monitor->m_writing));
	past_time = curretn_time - get_timestamp();
	pthread_mutex_lock(&(philo->m_time_last_meal));
	philo->time_last_meal -= past_time;
	pthread_mutex_unlock(&(philo->m_time_last_meal));

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