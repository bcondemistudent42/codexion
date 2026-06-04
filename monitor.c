/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcondemi <bcondemi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/04 12:09:30 by bcondemi          #+#    #+#             */
/*   Updated: 2026/06/04 19:02:34 by bcondemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

// temp
int check_finish(t_manager *manager);
int check_burnout(t_coder *coder);
void monitor_checker(void *the_manager);
int end_type_handler(t_manager *manager);
int check_burnout_all_coders(t_manager *manager);
void wait_for_start_manager(t_manager *manager);

int monitor(t_manager *manager)
{
	// to protect later
	pthread_create(&manager->manager_thread, NULL,
				   (void *)monitor_checker, (void *)(manager));
	create_thread(manager);
	// to protect create thread also
	while (manager->end_type == RUNNING)
		usleep(10);
	return (0);
}

void monitor_checker(void *the_manager)
{
	t_manager *manager;

	manager = (t_manager *)(the_manager);
	wait_for_start_manager(manager);
	while (end_type_handler(manager) == FALSE)
		usleep(10);
	return;
}

void wait_for_start_manager(t_manager *manager)
{
	pthread_mutex_lock(&manager->protect_nb_ready);
	manager->nb_ready++;
	if (manager->nb_ready == manager->utils_const[NB_CODERS])
		pthread_cond_signal(&manager->cond_ready);
	while (manager->check_ready == FALSE)
		pthread_cond_wait(&manager->routine_wait_start,
						  &manager->protect_nb_ready);
	pthread_mutex_unlock(&manager->protect_nb_ready);
}

int end_type_handler(t_manager *manager)
{
	if (check_burnout_all_coders(manager) == TRUE)
		return (TRUE);
	else if (check_finish(manager) == TRUE)
		return (TRUE);
	return (FALSE);
}

int check_finish(t_manager *manager)
{
	int i;
	int cnt;

	i = 0;
	cnt = 0;
	while (i < manager->utils_const[NB_CODERS])
	{
		pthread_mutex_lock(&manager->coders[i].coder_mutex);
		if (manager->coders[i].compile_cnt >= manager->utils_const[COMPILE_REQUIRED])
			cnt++;
		pthread_mutex_unlock(&manager->coders[i].coder_mutex);
		i++;
	}
	if (cnt == manager->utils_const[NB_CODERS])
	{
		pthread_mutex_lock(&manager->mutex_manager);
		manager->end_type = FINISHED;
		pthread_mutex_unlock(&manager->mutex_manager);
		return (TRUE);
	}
	return (FALSE);
}

int check_burnout_all_coders(t_manager *manager)
{
	int i;

	i = 0;
	while (i < manager->utils_const[NB_CODERS])
	{
		pthread_mutex_lock(&manager->coders[i].coder_mutex);
		if (check_burnout(&manager->coders[i]) == TRUE)
		{
			pthread_mutex_unlock(&manager->coders[i].coder_mutex);
			return (TRUE);
		}
		pthread_mutex_unlock(&manager->coders[i].coder_mutex);
		i++;
	}
	return (FALSE);
}

int check_burnout(t_coder *coder)
{
	long long norm;
	long long time_check;
	long long temp_last_compile;

	time_check = get_time();
	if (coder->id % 2 == 0)
		temp_last_compile = coder->last_compile - 1;
	else
		temp_last_compile = coder->last_compile;
	norm = time_check - coder->utils_const[TM_START];
	if (coder->manager->end_type != RUNNING)
		return (FALSE);
	else if (norm - temp_last_compile > coder->utils_const[TM_BURNOUT])
	{
		pthread_mutex_lock(&coder->manager->mutex_print);
		printf("%lld %d has burnout\n",norm, coder->id);
		pthread_mutex_unlock(&coder->manager->mutex_print);
		pthread_mutex_lock(&coder->manager->mutex_manager);
		coder->manager->end_type = BURNOUT_ERROR;
		pthread_mutex_unlock(&coder->manager->mutex_manager);
		return (TRUE);
	}
	return (FALSE);
}