/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shima <shima@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/08 11:12:36 by shima             #+#    #+#             */
/*   Updated: 2022/09/09 20:55:59 by shima            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <stdbool.h>
# include <sys/time.h>
# include <limits.h>

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
	pthread_t		*threads;
	t_philo			*philos;
	bool			is_error;
}	t_monitor;

// init.c
bool		init_monitor(int argc, char *argv[], t_monitor *monitor);
bool		init_philo(t_monitor *monitor);

// th_monitor.c
void		*monitor_routine(void *arg);

// th_philo.c
void		*philosopher_routine(void *arg);

// utility.c
void		print_log(int id, char *act_msg, pthread_mutex_t *m_writing);
int			ft_atoi(const char *str);
long long	get_timestamp(void);
void		all_free(t_monitor *monitor);

#endif