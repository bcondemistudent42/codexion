/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcondemi <bcondemi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/04 12:09:30 by bcondemi          #+#    #+#             */
/*   Updated: 2026/06/04 14:53:24 by bcondemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

// temp
int	is_finish(t_manager *manager);
int check_burnout(t_coder *coder);
void monitor_checker(void *the_manager);
int	check_burnout_all_coders(t_manager *manager);
void	wait_for_start_manager(t_manager *manager);

int monitor(t_manager *manager)
{
	// to protect later
	pthread_create(&manager->manager_thread, NULL,
		(void *) monitor_checker, (void *)(manager));
	create_thread(manager);
	// to protect create thread also
	while (manager->end_type == RUNNING)
		usleep(10);
	if (manager->end_type == BURNOUT_ERROR)
	{
		// faire stop tout les threds directement impossible de print quoi que ce soit
		// peut utilisier mutyex print
		// to free all
		pthread_join(manager->manager_thread, NULL);
	}
	if (manager->end_type == FINISHED)
	{
		// to free all
		pthread_join(manager->manager_thread, NULL);
	}
		return (0);
}

void monitor_checker(void *the_manager)
{
	t_manager *manager;

	manager = (t_manager *) (the_manager);
	wait_for_start_manager(manager);
	while (is_finish(manager) == FALSE)
		usleep(10);
	return ;
}

void	wait_for_start_manager(t_manager *manager)
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

int	is_finish(t_manager *manager)
{
	int	i;
	int output;

	i = 0;
	output = TRUE;
	check_burnout_all_coders(manager);
	if (manager->end_type == BURNOUT_ERROR)
		return (TRUE);
	while (i < manager->utils_const[NB_CODERS])
	{
		pthread_mutex_lock(&manager->coders[i].coder_mutex);
		if (manager->coders[i].compile_cnt < manager->utils_const[COMPILE_REQUIRED])
			output = FALSE;
		pthread_mutex_unlock(&manager->coders[i].coder_mutex);
		i++;
	}
	return (output);
}

int	check_burnout_all_coders(t_manager *manager)
{
	int	i;

	i = 0;
	while (i < manager->utils_const[NB_CODERS])
	{
		pthread_mutex_lock(&manager->coders[i].coder_mutex);
		if (check_burnout(&manager->coders[i]) == TRUE)
			return (-1);
		pthread_mutex_unlock(&manager->coders[i].coder_mutex);
		i++;
	}
	return (0);
}

int check_burnout(t_coder *coder)
{
	long long time_check;

	time_check = get_time();
	if (coder->manager->end_type != RUNNING)
		return (TRUE);
	if (coder->last_compile == -2 || coder->last_compile == -1)
		return (FALSE);
	else if (time_check - coder->last_compile > coder->utils_const[TM_BURNOUT])
	{
		pthread_mutex_lock(&coder->manager->mutex_print);
		printf("%lld %d has burnout\n", time_check -coder->utils_const[TM_START], coder->id);
		pthread_mutex_unlock(&coder->manager->mutex_print);
		coder->manager->end_type = BURNOUT_ERROR;
	}
	return (FALSE);
}