/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_thread.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcondemi <bcondemi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 15:53:03 by bcondemi          #+#    #+#             */
/*   Updated: 2026/06/03 20:49:03 by bcondemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

void compile(t_coder *coder);
void debug(t_coder *coder);
void refacto(t_coder *coder);


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
		if (can_compile(coder) == TRUE)
		{
			take_both_dongle(coder);
			compile(coder);
			// to lock and unlock with the coder mutex
			coder->compile_cnt++;
			// to lock and unlock with the coder mutex
			debug(coder);
			refacto(coder);
		}
		else
			usleep(10);
	}
}

int	make_thread_join(t_manager *manager, int index)
{
	int	i;

	i = 0;
	while (i < index)
	{
		// check succeded to join thread
		pthread_join(manager->coders[i].thread_id, NULL);
		i++;
	}
	return (0);
}

void	wait_for_start(t_coder *coder)
{
	pthread_mutex_lock(&coder->manager->protect_nb_ready);
	coder->manager->nb_ready++;
	if (coder->manager->nb_ready == coder->utils_const[NB_CODERS])
		pthread_cond_signal(&coder->manager->cond_ready);
	while (coder->manager->check_ready == FALSE)
		pthread_cond_wait(&coder->manager->routine_wait_start,
			&coder->manager->protect_nb_ready);
	pthread_mutex_unlock(&coder->manager->protect_nb_ready);
}


void	launch_thread(t_manager *manager)
{
	pthread_mutex_lock(&manager->protect_nb_ready);
	manager->utils_const[TM_START] = get_time();
	while (manager->nb_ready < manager->utils_const[NB_CODERS])
		pthread_cond_wait(&manager->cond_ready, &manager->protect_nb_ready);
	manager->check_ready = TRUE;
	pthread_cond_broadcast(&manager->routine_wait_start);
	pthread_mutex_unlock(&manager->protect_nb_ready);
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

void take_dongle(t_dongle *dongle, t_coder *coder)
{
	pthread_mutex_lock(&dongle->dongle_mtx);
	dongle->available = FALSE;
	pthread_mutex_lock(&coder->manager->mutex_print);
	printf("%lld %d has taken a dongle\n",
		get_time() - dongle->utils_const[TM_START], coder->id);
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

void release_dongle(t_dongle *dongle, long long time_release)
{
	pthread_mutex_lock(&dongle->dongle_mtx);
	(void)(time_release);
	dongle->last_time_used = time_release; // to see if do now or when release
	dongle->available = TRUE;
	swap_priority(dongle->queue);
	pthread_mutex_unlock(&dongle->dongle_mtx);
}

void release_both_dongle(t_coder *coder)
{
	long long time_released;

	time_released = get_time();
	release_dongle(coder->left, time_released);
	release_dongle(coder->right, time_released);
	coder->last_compile = time_released;
}

void	swap_priority(int queue[2])
{
	int	temp;

	temp = queue[0];
	queue[0] = queue[1];
	queue[1] = temp;
}

int	check_dongle(t_dongle *dongle, long long request_time, int coder_id)
{
	long long	free_at;

	free_at = dongle->last_time_used + dongle->utils_const[DONGLE_COOLDOWN];
	if (dongle->available == FALSE)
		return (FALSE);
	if (free_at > request_time)
		return (FALSE);
	if (dongle->priority_type == EDF)
	{
		// functions finds the min
	}
	else
	{
		if (dongle->queue[0] != coder_id)
			return (FALSE);
	}
	return (TRUE);
}


void compile(t_coder *coder)
{
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