/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shima <shima@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/27 10:40:17 by shima             #+#    #+#             */
/*   Updated: 2022/09/01 11:55:44 by shima            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>

typedef struct s_philo
{
	int	id;
	int right;
	int left;
	

}	t_philo;

typedef struct s_monitor
{
	int				number_of_philosophers;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	pthread_t		*ptherads;
	pthread_mutex_t	*forks;
	t_philo			*philos;
}	t_monitor;




int	ft_atoi(const char *str);

#endif