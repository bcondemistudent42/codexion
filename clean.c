/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcondemi <bcondemi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/04 23:39:20 by bcondemi          #+#    #+#             */
/*   Updated: 2026/06/04 23:39:39 by bcondemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

void	destroy_mutex_dongle(t_manager *manager, int max)
{
	int	i;

	i = 0;
	while (i < max)
	{
		pthread_mutex_destroy(&manager->dongles[i].dongle_mtx);
		i++;
	}
}

void	destroy_mutex_coders(t_manager *manager, int max)
{
	int	i;

	i = 0;
	while (i < max)
	{
		pthread_mutex_destroy(&manager->coders[i].coder_mutex);
		i++;
	}
}

void	destroy_const_mutex(t_manager *manager)
{
	pthread_mutex_destroy(&manager->protect_nb_ready);
	pthread_mutex_destroy(&manager->mutex_print);
	pthread_cond_destroy(&manager->cond_ready);
	pthread_cond_destroy(&manager->routine_wait_start);
}

void	final_clean(t_manager *manager)
{
	destroy_const_mutex(manager);
	destroy_mutex_dongle(manager, manager->utils_const[NB_CODERS]);
	destroy_mutex_coders(manager, manager->utils_const[NB_CODERS]);
	free(manager->coders);
	free(manager->dongles);
	pthread_mutex_destroy(&manager->mutex_manager);
	free(manager);
}
