/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_state.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcondemi <bcondemi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/04 12:02:48 by bcondemi          #+#    #+#             */
/*   Updated: 2026/06/04 15:42:51 by bcondemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

int compile(t_coder *coder)
{
	long long wake_up;
	long long actual_time;

	take_both_dongle(coder);
	if (coder->manager->end_type != RUNNING)
		return (-1);
	pthread_mutex_lock(&coder->manager->mutex_print);
	printf("%lld %d is compiling\n",get_time() - coder->utils_const[TM_START], coder->id);
	pthread_mutex_unlock(&coder->manager->mutex_print);
	actual_time = get_time();
	wake_up = actual_time + (coder->utils_const[TM_COMPILE]);
	while (actual_time < wake_up)
	{
		if (coder->manager->end_type != RUNNING)
		{
			release_both_dongle(coder);
			return (-1);
		}
		else
			usleep(10);
		actual_time = get_time();
	}
	release_both_dongle(coder);
	return (0);
	// faire reveiller chaque coder pour verifier que pas de burnout
}

int debug(t_coder *coder)
{
	long long wake_up;

	if (coder->manager->end_type != RUNNING)
		return (-1);
	pthread_mutex_lock(&coder->manager->mutex_print);
	printf("%lld %d is debugging\n",
	get_time() - coder->utils_const[TM_START], coder->id);
	pthread_mutex_unlock(&coder->manager->mutex_print);
	wake_up = get_time() + (coder->utils_const[TM_DEBUG]);
	while (get_time() < wake_up)
	{
		if (coder->manager->end_type != RUNNING)
			return (-1);
		else
			usleep(10);
	}
	return (0);
	// faire reveiller chaque coder pour verifier que pas de burnout
}

int refacto(t_coder *coder)
{
	long long wake_up;

	if (coder->manager->end_type != RUNNING)
		return (-1);
	pthread_mutex_lock(&coder->manager->mutex_print);
	printf("%lld %d is refactoring\n",
	get_time() - coder->utils_const[TM_START], coder->id);
	pthread_mutex_unlock(&coder->manager->mutex_print);
	wake_up = (get_time() + coder->utils_const[TM_REFACTO]);
	while (get_time() < wake_up)
	{
		if (coder->manager->end_type != RUNNING)
			return (-1);
		else
			usleep(10);
	}
	return (0);
	// faire reveiller chaque coder pour verifier que pas de burnout
}
