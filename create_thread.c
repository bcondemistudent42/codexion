/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_thread.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcondemi <bcondemi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 15:53:03 by bcondemi          #+#    #+#             */
/*   Updated: 2026/05/29 10:39:51 by bcondemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"


void my_function(void *manager);
int make_thread_join(t_manager *manager);

int	create_thread(int nb_thread, t_manager *manager)
{
	int i;

	i = 0;
	while (i < manager->utils_const[NB_CODERS])
	{
		pthread_create(&manager->coders[i]->thread_id, NULL, (void *) my_function, manager->coders[i]);
		// create and check if error
		i++;
	}
	pthread_mutex_lock(&manager->mutex_manager);
	while (manager->nb_ready < manager->utils_const[NB_CODERS])
		pthread_cond_wait(&manager->manager_sleep, &manager->mutex_manager);
	pthread_mutex_unlock(&manager->mutex_manager);
	pthread_mutex_lock(&manager->start_ready_mtx);
	pthread_cond_broadcast(&manager->start_cond);
	pthread_mutex_unlock(&manager->start_ready_mtx);

	make_thread_join(manager);
	return (0);
}


void my_function(void *my_coder)
{
	t_coder *coder;
	coder = (t_coder*)(my_coder);
	// waiting for all coders to be ready
	pthread_mutex_lock(&coder->manager->mutex_manager);
	coder->manager->nb_ready++;
	if (coder->manager->nb_ready == coder->manager->utils_const[NB_CODERS])
		pthread_cond_signal(&coder->manager->manager_sleep); // wake up main and make it launch all thread
	pthread_mutex_unlock(&coder->manager->mutex_manager);
	// to do a function wait until ready

	// starting true job of the thread, mutex to protect the values
	pthread_mutex_lock(&coder->manager->start_ready_mtx);
	pthread_cond_wait(&coder->manager->start_cond, &coder->manager->start_ready_mtx);
	printf("Created thread for coder: %d\n", coder->id);
	// to do the algorythm
	// checking if dongle available 	
	// compile 
	// debug 
	// refacto
	pthread_mutex_unlock(&coder->manager->start_ready_mtx);
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
	printf("CLOSED ALL THRADS\n");
	return (0);
}