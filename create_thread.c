/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_thread.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcondemi <bcondemi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 15:53:03 by bcondemi          #+#    #+#             */
/*   Updated: 2026/05/28 17:09:15 by bcondemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"


void my_function(void *manager);
int make_thread_join(t_manager *manager);

int	create_thread(int nb_thread, t_manager *manager)
{
	int		i;
	
	i = 0;
	while (i < nb_thread)
	{
		
		if (pthread_create(&manager->coders[i]->thread_id, NULL, (void *)my_function, manager->coders[i]) != 0) // to protect
		{
			// to free all and handle error
			return (-1);
		}
		// printf("i == %d\n", i);
		i++;
	}
	pthread_mutex_lock(&manager->manager_mutex);
	while (manager->nb_ready < manager->utils_const[NB_CODERS])
		 pthread_cond_wait(&manager->mutex_ready, &manager->manager_mutex);
	pthread_cond_broadcast(&manager->start_cond);
	make_thread_join(manager);
	return (0);

	// tant que simulation pas start et moniteur not created and threads not created wait
	// to do a function that will join all thread in the end
}


void my_function(void *my_coder)
{
	t_coder *casted_coder;
	pthread_mutex_t ready_mutex;

	casted_coder = (t_coder*)my_coder;
	ready_mutex = casted_coder->manager->mutex_ready;
	pthread_mutex_lock(&ready_mutex);
	casted_coder->manager->nb_ready++;
	if (casted_coder->manager->nb_ready == casted_coder->manager->utils_const[NB_CODERS])
		pthread_cond_signal(&casted_coder->manager->manager_mutex);
	pthread_mutex_unlock(&ready_mutex);
	
	// pthread_mutex_init(coder_mutex, NULL);
	// 
	// pthread_mutex_lock(coder_mutex);
	// pthread_cond_wait(casted_coder->start_cond, coder_mutex);
	
	// wait for all thread to be ready
	printf("thread created for coder nb == %d\n", casted_coder->id);
	return ;
	// checking if dongle available 	
	// pthread_mutex_unlock(coder_mutex);

	// compile 
	// debug 
	// refacto

}

int make_thread_join(t_manager *manager)
{
	int i;

	i = 0;
	while (i < manager->utils_const[NB_CODERS])
	{
		// check succeded to join thread
		pthread_join(manager->coders[i]->thread_id, NULL);
		i++;
	}
	return (0);
}