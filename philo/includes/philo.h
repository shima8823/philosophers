/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shima <shima@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/27 10:40:17 by shima             #+#    #+#             */
/*   Updated: 2022/09/07 13:52:12 by shima            ###   ########.fr       */
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
	int					id;
	int					right;
	int					left;
	int					count_eat;
	pthread_mutex_t		m_count_eat;
	bool				is_ate;
	long long			time_last_meal;
	pthread_mutex_t		m_time_last_meal;
	pthread_mutex_t		m_is_finish_act;
	pthread_t			thread;
	struct s_monitor	*monitor;
}	t_philo;

typedef struct s_monitor
{
	int				num_of_philos;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				times_philo_must_eat;
	int				count_philos_ate;
	pthread_mutex_t	m_count_philos_ate;
	pthread_mutex_t	m_writing;
	pthread_mutex_t	m_is_finish;
	pthread_mutex_t	*forks;
	t_philo			*philos;
}	t_monitor;




int	ft_atoi(const char *str, bool *is_valid);

#endif