/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcondemi <bcondemi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/04 12:03:52 by bcondemi          #+#    #+#             */
/*   Updated: 2026/06/05 12:01:40 by bcondemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

void	take_dongle(t_dongle *dongle, t_coder *coder)
{
	pthread_mutex_lock(&dongle->dongle_mtx);
	dongle->available = FALSE;
	pthread_mutex_lock(&coder->manager->mutex_print);
	printf("%lld %d has taken a dongle\n",
		get_time(), coder->id);
	pthread_mutex_unlock(&coder->manager->mutex_print);
	pthread_mutex_unlock(&dongle->dongle_mtx);
}

void	take_both_dongle(t_coder *coder)
{
	long long	time_took_dongle;

	time_took_dongle = get_time();
	take_dongle(coder->left, coder);
	take_dongle(coder->right, coder);
}

void	release_dongle(t_dongle *dongle, long long time_release)
{
	pthread_mutex_lock(&dongle->dongle_mtx);
	dongle->last_time_used = time_release;
	dongle->available = TRUE;
	swap_priority(dongle->queue);
	pthread_mutex_unlock(&dongle->dongle_mtx);
}

void	release_both_dongle(t_coder *coder)
{
	long long	time_released;

	time_released = get_time();
	release_dongle(coder->left, time_released);
	release_dongle(coder->right, time_released);
	pthread_mutex_lock(&coder->coder_mutex);
	coder->last_compile = time_released;
	pthread_mutex_unlock(&coder->coder_mutex);
}

void	swap_priority(t_coder *queue[2])
{
	t_coder	*temp;

	temp = queue[0];
	queue[0] = queue[1];
	queue[1] = temp;
}
