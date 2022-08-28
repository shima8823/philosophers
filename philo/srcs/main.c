/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shima <shima@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/27 10:39:50 by shima             #+#    #+#             */
/*   Updated: 2022/08/28 10:56:09 by shima            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

// int	main(int argc, char *argv[])
// {
// 	t_arg	info;

// 	info.number_of_philosophers = ft_atoi(argv[1]);
// 	info.number_of_philosophers = ft_atoi(argv[1]);
// }

void *f(void *p)
{
	int	*cnt;

	cnt = p;
	for (int i = 0; i < 10000; ++i)
		++(*cnt);
	return (NULL);
}

int main()
{
	int cnt = 0;
	pthread_t thread1;
	pthread_t thread2;

	// mainの処理とは別に働かせる。並列処理。
	pthread_create(&thread1, NULL, &f, &cnt);
	pthread_create(&thread2, NULL, &f, &cnt);

	// 終了するまで待機。pthread_createとセット
	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);

	printf("cnt: %d\n", cnt); // 20000がでると思った？
}