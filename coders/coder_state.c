/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_state.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcondemi <bcondemi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/04 12:02:48 by bcondemi          #+#    #+#             */
/*   Updated: 2026/06/05 13:35:14 by bcondemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

void	compile_print(t_coder *coder);

int	compile(t_coder *coder)
{
	long long	wake_up;
	long long	actual_time;

	take_both_dongle(coder);
	if (check_burnout_happen(coder) == -1)
		return (-1);
	pthread_mutex_lock(&coder->coder_mutex);
	coder->last_compile = get_time();
	pthread_mutex_unlock(&coder->coder_mutex);
	compile_print(coder);
	actual_time = get_time();
	wake_up = actual_time + (coder->utils_const[TM_COMPILE]);
	while (actual_time < wake_up)
	{
		pthread_mutex_lock(&coder->manager->mutex_manager);
		if (coder->manager->end_type != RUNNING)
			return (unlock_and_release(coder));
		pthread_mutex_unlock(&coder->manager->mutex_manager);
		actual_time = get_time();
	}
	release_both_dongle(coder);
	return (0);
}

int	debug(t_coder *coder)
{
	long long	wake_up;
	long long	actual_time;

	pthread_mutex_lock(&coder->manager->mutex_manager);
	if (coder->manager->end_type != RUNNING)
		return (-1);
	pthread_mutex_unlock(&coder->manager->mutex_manager);
	pthread_mutex_lock(&coder->manager->mutex_print);
	printf("%lld %d is debugging\n",
		get_time(), coder->id);
	pthread_mutex_unlock(&coder->manager->mutex_print);
	actual_time = get_time();
	wake_up = actual_time + (coder->utils_const[TM_DEBUG]);
	while (actual_time < wake_up)
	{
		pthread_mutex_lock(&coder->manager->mutex_manager);
		if (coder->manager->end_type != RUNNING)
		{
			pthread_mutex_unlock(&coder->manager->mutex_manager);
			return (-1);
		}
		pthread_mutex_unlock(&coder->manager->mutex_manager);
		actual_time = get_time();
	}
	return (0);
}

int	refacto(t_coder *coder)
{
	long long	wake_up;
	long long	actual_time;

	pthread_mutex_lock(&coder->manager->mutex_manager);
	if (coder->manager->end_type != RUNNING)
		return (-1);
	pthread_mutex_unlock(&coder->manager->mutex_manager);
	pthread_mutex_lock(&coder->manager->mutex_print);
	printf("%lld %d is refactoring\n",
		get_time(), coder->id);
	pthread_mutex_unlock(&coder->manager->mutex_print);
	actual_time = get_time();
	wake_up = actual_time + (coder->utils_const[TM_REFACTO]);
	while (actual_time < wake_up)
	{
		pthread_mutex_lock(&coder->manager->mutex_manager);
		if (coder->manager->end_type != RUNNING)
		{
			pthread_mutex_unlock(&coder->manager->mutex_manager);
			return (-1);
		}
		pthread_mutex_unlock(&coder->manager->mutex_manager);
		actual_time = get_time();
	}
	return (0);
}

int	check_burnout_happen(t_coder *coder)
{
	pthread_mutex_lock(&coder->manager->mutex_manager);
	if (coder->manager->end_type != RUNNING)
	{
		pthread_mutex_unlock(&coder->manager->mutex_manager);
		release_both_dongle(coder);
		return (-1);
	}
	pthread_mutex_unlock(&coder->manager->mutex_manager);
	return (0);
}

void	compile_print(t_coder *coder)
{
	pthread_mutex_lock(&coder->manager->mutex_print);
	printf("%lld %d is compiling\n",
		get_time(), coder->id);
	pthread_mutex_unlock(&coder->manager->mutex_print);
}
