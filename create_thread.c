/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_thread.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcondemi <bcondemi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 15:53:03 by bcondemi          #+#    #+#             */
/*   Updated: 2026/06/04 14:51:33 by bcondemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

int	create_thread(t_manager *manager)
{
	int	i;

	i = -1;
	while (++i < manager->utils_const[NB_CODERS])
	{
		if (pthread_create(&manager->coders[i].thread_id, NULL,
				(void *) my_function, (void *)(&manager->coders[i])) != 0)
		{
			printf("ERROR CREATING THREAD\n");
			pthread_mutex_lock(&manager->protect_nb_ready);
			manager->check_ready = TRUE;
			manager->thread_error = TRUE;
			pthread_cond_broadcast(&manager->routine_wait_start);
			pthread_mutex_unlock(&manager->protect_nb_ready);
			make_thread_join(manager, i);
			return (-1);
		}
	}
	launch_thread(manager);
	make_thread_join(manager, manager->utils_const[NB_CODERS]);
	return (0);
}

void	my_function(void *my_coder)
{
	t_coder	*coder;

	coder = (t_coder *)(my_coder);
	wait_for_start(coder);
	if (coder->manager->thread_error == TRUE)
		return ;
	if (coder->id % 2 == 0)
		usleep(1000);
	while (coder->compile_cnt < coder->utils_const[COMPILE_REQUIRED])
	{
		if (coder->manager->end_type != RUNNING) //i thought useful to see
			return ;
		else if (can_compile(coder) == TRUE)
		{
			compile(coder);
			pthread_mutex_lock(&coder->coder_mutex);
			coder->compile_cnt++;
			pthread_mutex_unlock(&coder->coder_mutex);
			debug(coder);
			refacto(coder);
		}
		else
			usleep(10);
	}
	// have to check all time if a burnout happen
}

int	can_compile(t_coder *coder)
{
	int			left;
	int			right;
	long long	request_time;

	request_time = get_time();
	pthread_mutex_lock(&coder->left->dongle_mtx);
	left = check_dongle(coder->left, request_time, coder->id);
	pthread_mutex_unlock(&coder->left->dongle_mtx);
	pthread_mutex_lock(&coder->right->dongle_mtx);
	right = check_dongle(coder->right, request_time, coder->id);
	pthread_mutex_unlock(&coder->right->dongle_mtx);
	if (left == TRUE && right == TRUE)
		return (TRUE);
	return (FALSE);
}
