/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_keeper.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcondemi <bcondemi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/29 16:23:50 by bcondemi          #+#    #+#             */
/*   Updated: 2026/06/02 21:49:27 by bcondemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

long long	get_time(void)
{
	struct timeval	start;

	gettimeofday(&start, NULL);
	return (((long long)start.tv_sec * 1000) + (start.tv_usec / 1000));
}
