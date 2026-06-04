/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcondemi <bcondemi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/04 12:06:34 by bcondemi          #+#    #+#             */
/*   Updated: 2026/06/04 23:59:20 by bcondemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

int	make_thread_join(t_manager *manager, int index)
{
	int	i;

	i = 0;
	while (i < index)
	{
		pthread_join(manager->coders[i].thread_id, NULL);
		i++;
	}
	return (0);
}

void	wait_for_start(t_coder *coder)
{
	pthread_mutex_lock(&coder->manager->protect_nb_ready);
	coder->manager->nb_ready++;
	if (coder->manager->nb_ready == coder->utils_const[NB_CODERS])
		pthread_cond_signal(&coder->manager->cond_ready);
	while (coder->manager->check_ready == FALSE)
		pthread_cond_wait(&coder->manager->routine_wait_start,
			&coder->manager->protect_nb_ready);
	pthread_mutex_unlock(&coder->manager->protect_nb_ready);
}

void	launch_thread(t_manager *manager)
{
	pthread_mutex_lock(&manager->protect_nb_ready);
	manager->utils_const[TM_START] = get_time();
	while (manager->nb_ready < manager->utils_const[NB_CODERS])
		pthread_cond_wait(&manager->cond_ready, &manager->protect_nb_ready);
	manager->check_ready = TRUE;
	pthread_cond_broadcast(&manager->routine_wait_start);
	pthread_mutex_unlock(&manager->protect_nb_ready);
}

int	unlock_and_release(t_coder *coder)
{
	pthread_mutex_unlock(&coder->manager->mutex_manager);
	release_both_dongle(coder);
	return (-1);
}
