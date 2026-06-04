/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_one.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcondemi <bcondemi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/05 00:26:08 by bcondemi          #+#    #+#             */
/*   Updated: 2026/06/05 00:26:59 by bcondemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

int	custom_clean(t_manager *manager)
{
	pthread_cond_destroy(&manager->cond_ready);
	pthread_cond_destroy(&manager->routine_wait_start);
	pthread_mutex_destroy(&manager->protect_nb_ready);
	pthread_mutex_destroy(&manager->mutex_print);
	return (thread_error());
}

int	big_check_mutex(t_manager *manager)
{
	if (pthread_mutex_init(&manager->protect_nb_ready, NULL) != 0)
		return (thread_error());
	if (pthread_mutex_init(&manager->mutex_print, NULL) != 0)
	{
		pthread_mutex_destroy(&manager->protect_nb_ready);
		return (thread_error());
	}
	if (pthread_cond_init(&manager->cond_ready, NULL) != 0)
	{
		pthread_mutex_destroy(&manager->protect_nb_ready);
		pthread_mutex_destroy(&manager->mutex_print);
		return (thread_error());
	}
	if (pthread_cond_init(&manager->routine_wait_start, NULL) != 0)
	{
		pthread_cond_destroy(&manager->cond_ready);
		pthread_mutex_destroy(&manager->protect_nb_ready);
		pthread_mutex_destroy(&manager->mutex_print);
		return (thread_error());
	}
	if (pthread_mutex_init(&manager->mutex_manager, NULL) != 0)
		return (custom_clean(manager));
	return (0);
}

int	free_co_do_ma(t_manager *manager)
{
	free(manager->dongles);
	free(manager->coders);
	free(manager);
	return (-1);
}
