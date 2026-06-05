/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_priority.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcondemi <bcondemi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/04 12:05:18 by bcondemi          #+#    #+#             */
/*   Updated: 2026/06/05 20:08:26 by bcondemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	find_closest_burnout(t_dongle *dongle)
{
	int		output;
	t_coder	*first_to_lock;
	t_coder	*second_to_lock;

	if (dongle->queue[0]->id < dongle->queue[1]->id)
	{
		first_to_lock = dongle->queue[0];
		second_to_lock = dongle->queue[1];
	}
	else
	{
		first_to_lock = dongle->queue[1];
		second_to_lock = dongle->queue[0];
	}
	pthread_mutex_lock(&first_to_lock->coder_mutex);
	pthread_mutex_lock(&second_to_lock->coder_mutex);
	output = litlle_burnout(dongle);
	if (output == EQUAL_BURNOUT)
	{
		output = handle_burnout(dongle);
	}
	pthread_mutex_unlock(&second_to_lock->coder_mutex);
	pthread_mutex_unlock(&first_to_lock->coder_mutex);
	return (output);
}

int	handle_burnout(t_dongle *dongle)
{
	int	output;

	if (dongle->queue[0]->compile_cnt < dongle->queue[1]->compile_cnt)
		output = (dongle->queue[0]->id);
	else if (dongle->queue[1]->compile_cnt < dongle->queue[0]->compile_cnt)
		output = (dongle->queue[1]->id);
	else
	{
		if (dongle->queue[0]->id < dongle->queue[1]->id)
			output = (dongle->queue[0]->id);
		else
			output = (dongle->queue[1]->id);
	}
	return (output);
}

int	litlle_burnout(t_dongle *dongle)
{
	int			output;
	long long	first;
	long long	second;

	first = dongle->queue[0]->last_compile;
	first += (dongle->utils_const[TM_BURNOUT]) * 1000;
	second = dongle->queue[1]->last_compile;
	second += (dongle->utils_const[TM_BURNOUT]) * 1000;
	if (first < second)
		output = (dongle->queue[0]->id);
	else if (second < first)
		output = (dongle->queue[1]->id);
	else
		output = (EQUAL_BURNOUT);
	return (output);
}

int	check_dongle(t_dongle *dongle, int coder_id)
{
	long long	free_at;

	free_at = dongle->last_time_used + dongle->utils_const[DONGLE_CD];
	if (dongle->available == FALSE)
		return (FALSE);
	if (free_at > get_time())
		return (FALSE);
	if (*dongle->priority_type == EDF)
	{
		if (find_closest_burnout(dongle) != coder_id)
			return (FALSE);
	}
	else
	{
		if (dongle->queue[0]->id != coder_id)
			return (FALSE);
	}
	return (TRUE);
}
