/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_keeper.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcondemi <bcondemi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/29 16:23:50 by bcondemi          #+#    #+#             */
/*   Updated: 2026/06/01 11:29:01 by bcondemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

long long get_time()
{
	struct timeval start;

	gettimeofday(&start, NULL);
	return (((long long)start.tv_sec * 1000) + (start.tv_usec / 1000));
}


void ft_set_coders_time(t_manager *manager)
{
	int i;

	i = 0;
	pthread_mutex_lock(&manager->var_mutex);
	while (i < manager->utils_const[NB_CODERS])
	{
		manager->coders[i]->last_compile = manager->time_start;
		i++;
	}
	pthread_mutex_unlock(&manager->var_mutex);
}

// long long get_diff_time(long long actual_time, t_manager manager)
// {
	
// }