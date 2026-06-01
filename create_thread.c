/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_thread.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcondemi <bcondemi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 15:53:03 by bcondemi          #+#    #+#             */
/*   Updated: 2026/06/01 20:38:06 by bcondemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"


void my_function(void *manager);
int make_thread_join(t_manager *manager, int index);


// temp declaration :
int can_compile(t_coder *coder);
void handle_dongle_size(t_coder *coder);
void take_dongle(t_coder *coder);
void delete_coder_queue(t_coder *coder);
void release_dongle(t_coder *coder);

int	create_thread(t_manager *manager)
{
	int i;

	i = 0;
	printf("\nCreating Thread: \n");
	while (i < manager->utils_const[NB_CODERS])
	{
		if (pthread_create(&manager->coders[i]->thread_id, NULL, (void *) my_function, manager->coders[i]) != 0)
		{
			// to handle this error case later, delete mutex and everything
			// to see with Adrien on monday
			printf("ERROR CREATING THREAD");
			make_thread_join(manager, i);
			return (-1);
		}
		// create and check if error
		i++;
	}
	pthread_mutex_lock(&manager->mutex_manager);
	while (manager->nb_ready < manager->utils_const[NB_CODERS])
		pthread_cond_wait(&manager->manager_sleep, &manager->mutex_manager);
	pthread_mutex_unlock(&manager->mutex_manager);
	manager->time_start = get_time();
	ft_set_coders_time(manager);
	pthread_mutex_lock(&manager->start_ready_mtx);
	// int i = 0;
	// while (i < manager->utils_const[NB_CODERS])
	// {
		// printf(manager->coders[i]->);
	// }
	pthread_cond_broadcast(&manager->start_cond);
	pthread_mutex_unlock(&manager->start_ready_mtx);

	make_thread_join(manager, manager->utils_const[NB_CODERS]);
	printf("CLOSED ALL THREADS\n\n");
	return (0);
}


void my_function(void *my_coder)
{
	t_coder *coder;

	coder = (t_coder*)(my_coder);
	pthread_mutex_lock(&coder->manager->mutex_manager);
	coder->manager->nb_ready++;
	if (coder->manager->nb_ready == coder->manager->utils_const[NB_CODERS])
		pthread_cond_signal(&coder->manager->manager_sleep); // wake up main and make it launch all thread
	pthread_mutex_unlock(&coder->manager->mutex_manager);

	pthread_mutex_lock(&coder->manager->start_ready_mtx);
	pthread_cond_wait(&coder->manager->start_cond, &coder->manager->start_ready_mtx);
	pthread_mutex_unlock(&coder->manager->start_ready_mtx);
	while (coder->compile_cnt < coder->utils_const[COMPILE_REQUIRED])
	{
		if (coder->id % 2 == 0)
			usleep(20000);
		
		if (can_compile(coder) == TRUE)
			{
				pthread_mutex_lock(&coder->manager->dongle_mutex);
				take_dongle(coder);
				coder->compile_cnt++;
				coder->last_compile = get_time() - coder->manager->time_start;
				release_dongle(coder);
				pthread_mutex_lock(&coder->manager->print_mutex);
				printf("%lld %d has taken a dongle\n", get_time() - coder->manager->time_start, coder->id);
				printf("%lld %d has taken a dongle\n", get_time() - coder->manager->time_start, coder->id);
				usleep(coder->utils_const[TM_COMPILE] * 1000);
				printf("%lld %d is compiling \n", get_time() - coder->manager->time_start, coder->id);
				pthread_mutex_unlock(&coder->manager->dongle_mutex);
				
				usleep(coder->utils_const[TM_DEBUG] * 1000);
				printf("%lld %d is debugging \n", get_time() - coder->manager->time_start, coder->id);
				usleep(coder->utils_const[TM_REFACTO] * 1000);
				printf("%lld %d is refactoring \n", get_time() - coder->manager->time_start, coder->id);
				pthread_mutex_unlock(&coder->manager->print_mutex);
			}
		}
}


int make_thread_join(t_manager *manager, int index)
{
	int i;

	i = 0;
	while (i < index)
	{
		// check succeded to join thread
		pthread_join(manager->coders[i]->thread_id, NULL);
		i++;
	}
	return (0);
}


int can_compile(t_coder *coder)
{
	handle_dongle_size(coder);
	if (coder->left->available == FALSE || coder->right->available == FALSE)
		return (FALSE);
	if (get_time() - coder->left->last_used < coder->utils_const[DONGLE_COOLDOWN])
		return (FALSE);
	if (get_time() - coder->right->last_used < coder->utils_const[DONGLE_COOLDOWN])
		return (FALSE);
	if (coder->left->queue[0] == coder->id && coder->right->queue[0] == coder->id)
		return (TRUE);
	return (FALSE);
}


void handle_dongle_size(t_coder *coder)
{
	if (coder->left->queue_size == 0)
	{
		coder->left->queue[0] = coder->id;
		coder->left->queue_size++;
	}
	else if (coder->left->queue_size == 1)
	{
		coder->left->queue[1] = coder->id;
		coder->left->queue_size++;
	}
	if (coder->right->queue_size == 0)
	{
		coder->right->queue[0] = coder->id;
		coder->right->queue_size++;
	}
	else if (coder->right->queue_size == 1)
	{
		coder->right->queue[1] = coder->id;
		coder->right->queue_size++;
	}
}


void take_dongle(t_coder *coder)
{
	coder->left->available = FALSE;
	coder->right->available = FALSE;
}


void delete_coder_queue(t_coder *coder)
{
	if (coder->left->queue_size == 0)
		return ;
	else if (coder->left->queue_size == 1)
		coder->left->queue_size--;
	else
	{
		coder->left->queue[0] = coder->left->queue[1];
		coder->left->queue_size--;
	}
	
	if (coder->right->queue_size == 0)
		return ;
	else if (coder->right->queue_size == 1)
		coder->right->queue_size--;
	else
	{
		coder->right->queue[0] = coder->right->queue[1];
		coder->right->queue_size--;
	}
}

void release_dongle(t_coder *coder)
{
	coder->left->last_used = get_time();
	coder->right->last_used = get_time();
	coder->left->available = TRUE;
	coder->right->available = TRUE;
	delete_coder_queue(coder);
}