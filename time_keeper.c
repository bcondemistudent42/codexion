/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_keeper.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcondemi <bcondemi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/29 16:23:50 by bcondemi          #+#    #+#             */
/*   Updated: 2026/06/01 10:44:41 by bcondemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

long long get_time()
{
	long long result_ms;
	struct timeval start;

    if (gettimeofday(&start, NULL) != 0) {
        printf("gettimeofday start failed");
        return (-1);
    }
	result_ms = ((long long)start.tv_sec * 1000) + (start.tv_usec / 1000);
	return (result_ms);
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