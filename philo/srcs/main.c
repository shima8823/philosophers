/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shima <shima@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/27 10:39:50 by shima             #+#    #+#             */
/*   Updated: 2022/09/08 11:05:26 by shima            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

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