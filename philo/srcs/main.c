/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shima <shima@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/27 10:39:50 by shima             #+#    #+#             */
/*   Updated: 2022/09/01 12:02:07 by shima            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

bool	init_monitor(int argc, char *argv[], t_monitor *monitor);
bool	init_philo(t_monitor *monitor, t_philo **philo);
void	*philosophers(void *arg);

int	main(int argc, char *argv[])
{
	t_monitor	monitor;
	t_philo		*philo;
	int			i;

	
	init_monitor(argc, argv, &monitor);
	printf("%d", monitor.number_of_philosophers);
	init_philo(&monitor, &philo);
	monitor.philos = philo;
	
	i = 0;
	while (i < monitor.number_of_philosophers)
	{
		pthread_create(&monitor.ptherads[i], NULL, philosophers, &monitor);
		i++;
	}

	return (0);
}

bool init_monitor(int argc, char *argv[], t_monitor *monitor)
{
	int i;
	if (argc != 5 && argc != 6)
		return (false);
	monitor->number_of_philosophers = ft_atoi(argv[1]);
	monitor->time_to_die = ft_atoi(argv[2]);
	monitor->time_to_eat = ft_atoi(argv[3]);
	monitor->time_to_sleep = ft_atoi(argv[4]);
	monitor->forks = malloc(sizeof(pthread_mutex_t *) * monitor->number_of_philosophers);
	monitor->ptherads = malloc(sizeof(pthread_t *) * monitor->number_of_philosophers);

	i = 0;
	while (i < monitor->number_of_philosophers)
	{
		pthread_mutex_init(&(monitor->forks[i]), NULL);
		i++;
	}
	return (true);
}

bool init_philo(t_monitor *monitor, t_philo **philo)
{
	int	i;
	
	philo = malloc(sizeof(t_philo *) * monitor->number_of_philosophers);
	if (!philo)
		return (false);
	while (i < monitor->number_of_philosophers)
	{
		philo[i] = malloc(sizeof(t_philo));
		if (!(philo[i]))
			return (false);
		philo[i]->id = i + 1;
		philo[i]->right = i;
		philo[i]->left = i + 1;

		if (philo[i]->left == monitor->number_of_philosophers)
			philo[i]->left = 0;
		
		
		i++;
	}
	return (true);
}

void	*philosophers(void *arg)
{
	t_monitor *monitor;

	monitor = arg;
	return (NULL);
}
