/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shima <shima@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/27 10:40:17 by shima             #+#    #+#             */
/*   Updated: 2022/09/05 10:55:14 by shima            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>
#include <sys/time.h>

typedef struct s_philo
{
	int	id;
	int right;
	int left;
	long long time_last_meal;
	
	pthread_t		thread;
	struct s_monitor	*monitor;
}	t_philo;

typedef struct s_monitor
{
	int				num_of_philos;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	
	
	pthread_mutex_t	*forks;
	t_philo			*philos;
}	t_monitor;




int	ft_atoi(const char *str);

#endif