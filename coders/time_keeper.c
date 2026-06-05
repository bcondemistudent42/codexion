/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_keeper.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcondemi <bcondemi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/29 16:23:50 by bcondemi          #+#    #+#             */
/*   Updated: 2026/06/05 11:55:52 by bcondemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"


static struct timeval	*get_start_time(void)
{
	static struct timeval	start_time;

	return (&start_time);
}

void	start_timestamp(void)
{
	gettimeofday(get_start_time(), NULL);
}

long long	get_time(void)
{
	struct timeval			current_time;
	struct timeval			*start_time;
	long					seconds;
	long					microseconds;
	long long						ms_time;

	start_time = get_start_time();
	gettimeofday(&current_time, NULL);
	seconds = current_time.tv_sec - start_time->tv_sec;
	microseconds = current_time.tv_usec - start_time->tv_usec;
	ms_time = (seconds * 1000) + (microseconds / 1000);
	return (ms_time);
}