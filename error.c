/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcondemi <bcondemi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 14:17:16 by bcondemi          #+#    #+#             */
/*   Updated: 2026/06/01 11:20:45 by bcondemi         ###   ########.fr       */
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

void	ft_free_coders(t_coder **coders, int index_to_stop)
{
	int	i;

	i = 0;
	while (i < index_to_stop)
	{
		free(coders[i]);
		i++;
	}
}


int free_coder_and_manager_with_error(t_coder **coders, t_manager *manager)
{
	free(coders);
	free(manager);
	return (allocation_error());
}


int free_coder_and_manager(t_coder **coders, t_manager *manager)
{
	free(coders);
	free(manager);
	return (-1);
}

void ft_big_free(t_manager *manager)
{
	pthread_cond_destroy(&manager->start_cond);
	pthread_cond_destroy(&manager->manager_sleep);
	pthread_mutex_destroy(&manager->var_mutex);
	pthread_mutex_destroy(&manager->start_ready_mtx);
	pthread_mutex_destroy(&manager->mutex_manager);
	free(manager->dongles);
	ft_free_coders(manager->coders, manager->utils_const[NB_CODERS]);
	free_coder_and_manager(manager->coders, manager);
}