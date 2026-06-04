/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcondemi <bcondemi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/04 12:09:30 by bcondemi          #+#    #+#             */
/*   Updated: 2026/06/05 00:32:22 by bcondemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

int	monitor(t_manager *manager)
{
	if (pthread_create(&manager->manager_thread, NULL,
			(void *)monitor_checker, (void *)(manager)) != 0)
		return (thread_error());
	if (create_thread(manager) == -1)
	{
		pthread_mutex_lock(&manager->mutex_manager);
		manager->end_type = FINISHED;
		pthread_mutex_unlock(&manager->mutex_manager);
		pthread_join(manager->manager_thread, NULL);
		return (-1);
	}
	pthread_join(manager->manager_thread, NULL);
	while (manager->end_type == RUNNING)
		usleep(1000);
	return (0);
}

void	monitor_checker(void *the_manager)
{
	t_manager	*manager;

	manager = (t_manager *)(the_manager);
	if (manager->thread_error == TRUE)
		return ;
	wait_for_start_manager(manager);
	while (end_type_handler(manager) == FALSE)
		usleep(1000);
	return ;
}

void	wait_for_start_manager(t_manager *manager)
{
	pthread_mutex_lock(&manager->protect_nb_ready);
	if (manager->nb_ready == manager->utils_const[NB_CODERS])
		pthread_cond_signal(&manager->cond_ready);
	while (manager->check_ready == FALSE)
		pthread_cond_wait(&manager->routine_wait_start,
			&manager->protect_nb_ready);
	pthread_mutex_unlock(&manager->protect_nb_ready);
}
