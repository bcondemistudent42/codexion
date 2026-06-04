/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_state.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcondemi <bcondemi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/04 12:02:48 by bcondemi          #+#    #+#             */
/*   Updated: 2026/06/04 12:02:57 by bcondemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

void compile(t_coder *coder)
{
	take_both_dongle(coder);
	pthread_mutex_lock(&coder->manager->mutex_print);
	printf("%lld %d is compiling\n",get_time() - coder->utils_const[TM_START], coder->id);
	pthread_mutex_unlock(&coder->manager->mutex_print);
	usleep(coder->utils_const[TM_COMPILE] * 1000);
	release_both_dongle(coder);
}

void debug(t_coder *coder)
{
	pthread_mutex_lock(&coder->manager->mutex_print);
	printf("%lld %d is debugging\n",
	get_time() - coder->utils_const[TM_START], coder->id);
	pthread_mutex_unlock(&coder->manager->mutex_print);
	usleep(coder->utils_const[TM_DEBUG] * 1000);
}

void refacto(t_coder *coder)
{
	pthread_mutex_lock(&coder->manager->mutex_print);
	printf("%lld %d is refactoring\n",
	get_time() - coder->utils_const[TM_START], coder->id);
	pthread_mutex_unlock(&coder->manager->mutex_print);
	usleep(coder->utils_const[TM_REFACTO]* 1000);
}