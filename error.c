/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcondemi <bcondemi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 14:17:16 by bcondemi          #+#    #+#             */
/*   Updated: 2026/06/03 15:23:40 by bcondemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

int	ft_error(void)
{
	printf("Error Usage:\n");
	printf("int: number_of_coders\n");
	printf("int: time_to_burnout\n");
	printf("int: time_to_compile\n");
	printf("int: time_to_debug\n");
	printf("int: time_to_refactor\n");
	printf("int: number_of_compiles_required\n");
	printf("int: dongle_cooldown\n");
	printf("scheduler: {'fifo' or 'edf'}\n");
	return (-1);
}

int	allocation_error(void)
{
	printf("A malloc failed allocating memory\n");
	return (-1);
}

int	thread_error(void)
{
	printf("A mutex init faile\n");
	return (-1);
}

void destroy_mutex_dongle(t_manager *manager, int max)
{
	int i;

	i = 0;
	while (i < max)
	{
		pthread_mutex_destroy(&manager->dongles[i].dongle_mtx);
		i++;
	}
}

void destroy_const_mutex(t_manager *manager)
{
	pthread_mutex_destroy(&manager->protect_nb_ready);
	pthread_mutex_destroy(&manager->mutex_print);
	pthread_cond_destroy(&manager->cond_ready);
	pthread_cond_destroy(&manager->routine_wait_start);
}