/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utility.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shima <shima@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/27 11:28:55 by shima             #+#    #+#             */
/*   Updated: 2022/09/09 11:44:23 by shima            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

static int	ft_isdigit(int c);

long long	get_timestamp(void)
{
	struct timeval	tv;
	if (gettimeofday(&tv, NULL) == -1)
		return (-1);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void	all_free(t_monitor *monitor)
{
	free(monitor->forks);
	free(monitor->thread);
	free(monitor->philos);
}

int	ft_atoi(const char *str)
{
	long			num;
	size_t			i;

	num = 0;
	i = 0;
	if (!str[i])
		return (-1);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (-1);
		num = num * 10 + str[i++] - '0';
		if (num > INT_MAX)
			return (-1);
	}
	return ((int)num);
}

static int	ft_isdigit(int c)
{
	if ('0' <= c && c <= '9')
		return (1);
	return (0);
}
