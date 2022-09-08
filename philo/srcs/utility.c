/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utility.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shima <shima@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/27 11:28:55 by shima             #+#    #+#             */
/*   Updated: 2022/09/08 11:04:05 by shima            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

static int	ft_isspace(int c);
int			ft_isdigit(int c);

long long	get_timestamp(void)
{
	struct timeval	tv;
	if (gettimeofday(&tv, NULL) == -1)
		return (-1);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

int	ft_atoi(const char *str, bool *is_valid)
{
	size_t			digit;
	unsigned long	num;
	size_t			i;

	digit = 0;
	num = 0;
	i = 0;
	while (ft_isspace(str[i]))
		i++;
	while (str[i] == '0')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
		{
			*is_valid = false;
			return (-1);
		}
		num = num * 10 + str[i++] - '0';
		if (++digit > 19 || num > __LONG_MAX__)
			return ((int)__LONG_MAX__);
	}
	return ((int)num);
}

static int	ft_isspace(int c)
{
	if (('\t' <= c && c <= '\r') || c == ' ')
		return (true);
	return (false);
}

int	ft_isdigit(int c)
{
	if ('0' <= c && c <= '9')
		return (1);
	return (0);
}
