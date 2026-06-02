/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_thread.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcondemi <bcondemi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 15:53:03 by bcondemi          #+#    #+#             */
/*   Updated: 2026/06/02 12:14:39 by bcondemi         ###   ########.fr       */
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

// v2 temp
void	wait_for_start(t_coder *coder);
void launch_thread(t_manager *manager);
int can_compile(t_coder *coder);
int	check_dongle(t_dongle *dongle, long long request_time, int coder_id);

int	create_thread(t_manager *manager)
{
	int i;

	i = -1;
	while (++i < manager->utils_const[NB_CODERS])
	{
		if (pthread_create(&manager->coders[i]->thread_id, NULL, (void *) my_function, manager->coders[i]) != 0)
		{
			// to handle this error case later, delete mutex and everything
			// to see with Adrien on monday
			printf("ERROR CREATING THREAD");
			make_thread_join(manager, i);
			return (-1);
		}
	}
	launch_thread(manager);
	make_thread_join(manager, manager->utils_const[NB_CODERS]);
	return (0);
}


void my_function(void *my_coder)
{
	t_coder *coder;

	coder = (t_coder*)(my_coder);
	wait_for_start(coder);
	// printf("Started thread %d at time %lld\n", coder->id, get_time());
	// pthread_mutex_lock(&coder->manager->mutex_print);
	// printf("Created thread %d at time %lld\n", coder->id, get_time());
	// pthread_mutex_unlock(&coder->manager->mutex_print);
	// to see if using mutex print
	while (coder->compile_cnt < coder->utils_const[COMPILE_REQUIRED])
	{
		// must init the queue of dongles before, so like this pass the can compile stuff
		printf("Coder: %d at time %lld can compile %d\n", coder->id, get_time(), can_compile(coder));
		coder->compile_cnt++;
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



void	wait_for_start(t_coder *coder)
{
	pthread_mutex_lock(&coder->manager->protect_nb_ready);
	coder->manager->nb_ready++;
	if (coder->manager->nb_ready == coder->utils_const[NB_CODERS])
	pthread_cond_signal(&coder->manager->cond_ready);
	
	// to wait broadcast from main
	// pthread_mutex_lock(&coder->manager->start_all_thread);
	while (coder->manager->check_ready == FALSE)
	{
		pthread_cond_wait(&coder->manager->routine_wait_start, &coder->manager->protect_nb_ready);
	}
	pthread_mutex_unlock(&coder->manager->protect_nb_ready);
}


void launch_thread(t_manager *manager)
{
	pthread_mutex_lock(&manager->protect_nb_ready);
	while (manager->nb_ready < manager->utils_const[NB_CODERS])
		pthread_cond_wait(&manager->cond_ready, &manager->protect_nb_ready);
	manager->check_ready = TRUE;
	pthread_cond_broadcast(&manager->routine_wait_start);
	pthread_mutex_unlock(&manager->protect_nb_ready);
}


int can_compile(t_coder *coder)
{
	int left;
	int right;
	long long request_time;

	request_time = get_time();
	left = check_dongle(coder->left, request_time, coder->id);
	right = check_dongle(coder->right, request_time, coder->id);
	if (left == TRUE && right == TRUE)
		return (TRUE);
	return (FALSE);
	// to call for left and right
}


// void handle_dongle_size(t_coder *coder)
// {

// }


// void take_dongle(t_coder *coder)
// {

// }


// void delete_coder_queue(t_coder *coder)
// {

// }

// void release_dongle(t_coder *coder)
// {

// }


int	check_dongle(t_dongle *dongle, long long request_time, int coder_id)
{
	pthread_mutex_lock(&dongle->dongle_mtx);
	if (dongle->available == FALSE)
	{
		pthread_mutex_unlock(&dongle->dongle_mtx);
		return (FALSE);
	}
	if (dongle->last_time_used - request_time < dongle->utils_const[DONGLE_COOLDOWN])
	{
		pthread_mutex_unlock(&dongle->dongle_mtx);
		return (FALSE);
	}
	if (dongle->queue[0] != coder_id)
	{
		pthread_mutex_unlock(&dongle->dongle_mtx);
		return (FALSE);
	}
	pthread_mutex_unlock(&dongle->dongle_mtx);
	return (TRUE);
}
