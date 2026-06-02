/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_thread.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcondemi <bcondemi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 15:53:03 by bcondemi          #+#    #+#             */
/*   Updated: 2026/06/02 19:02:20 by bcondemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"


void my_function(void *manager);
int make_thread_join(t_manager *manager, int index);


// temp declaration :
int can_compile(t_coder *coder);
void delete_coder_queue(t_coder *coder);

// v2 temp
void take_dongle(t_dongle *dongle, long long time_dongle_taken, t_coder *coder);
void release_dongle(t_dongle *dongle, long long time_release);
void swap_priority(int queue[2]);
void	wait_for_start(t_coder *coder);
void launch_thread(t_manager *manager);
int can_compile(t_coder *coder);
void	take_both_dongle(t_coder *coder);
int	check_dongle(t_dongle *dongle, long long request_time, int coder_id);
void release_both_dongle(t_coder *coder);

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
	if (coder->id % 2 == 0)
		usleep(2000);
	while (coder->compile_cnt < coder->utils_const[COMPILE_REQUIRED])
	{
		// must init the queue of dongles before, so like this pass the can compile stuff
		if (can_compile(coder) == TRUE)
		{
			take_both_dongle(coder);
			pthread_mutex_lock(&coder->manager->mutex_print);
			printf("%lld %d is compiling\n",get_time() - coder->utils_const[TM_START], coder->id);
			pthread_mutex_unlock(&coder->manager->mutex_print);
			usleep(coder->utils_const[TM_COMPILE]);
			release_both_dongle(coder);
			coder->compile_cnt++;
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



void	wait_for_start(t_coder *coder)
{
	pthread_mutex_lock(&coder->manager->protect_nb_ready);
	coder->manager->nb_ready++;
	if (coder->manager->nb_ready == coder->utils_const[NB_CODERS])
	pthread_cond_signal(&coder->manager->cond_ready);
	
	while (coder->manager->check_ready == FALSE)
		pthread_cond_wait(&coder->manager->routine_wait_start, &coder->manager->protect_nb_ready);
	pthread_mutex_unlock(&coder->manager->protect_nb_ready);
}


void launch_thread(t_manager *manager)
{
	pthread_mutex_lock(&manager->protect_nb_ready);
	manager->utils_const[TM_START] = get_time();
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
	pthread_mutex_lock(&coder->left->dongle_mtx);
	left = check_dongle(coder->left, request_time, coder->id);
	pthread_mutex_unlock(&coder->left->dongle_mtx);
	
	pthread_mutex_lock(&coder->right->dongle_mtx);
	right = check_dongle(coder->right, request_time, coder->id);
	pthread_mutex_unlock(&coder->right->dongle_mtx);

	if (left == TRUE && right == TRUE)
		return (TRUE);
	return (FALSE);
	// to call for left and right
}


void take_dongle(t_dongle *dongle, long long time_dongle_taken, t_coder *coder)
{
	pthread_mutex_lock(&dongle->dongle_mtx);
	dongle->available = FALSE;
	(void)time_dongle_taken;
	// dongle->last_time_used = time_dongle_taken; // to see if do now or when release
	pthread_mutex_lock(&coder->manager->mutex_print);
	printf("%lld %d has taken a dongle\n", get_time() - dongle->utils_const[TM_START], coder->id);
	pthread_mutex_unlock(&coder->manager->mutex_print);
	pthread_mutex_unlock(&dongle->dongle_mtx);
}

void	take_both_dongle(t_coder *coder)
{
	long long time_took_dongle;

	time_took_dongle = get_time();
	take_dongle(coder->left, time_took_dongle, coder);
	take_dongle(coder->right, time_took_dongle, coder);
}



// void delete_coder_queue(t_coder *coder)
// {

// }

void release_dongle(t_dongle *dongle, long long time_release)
{
	pthread_mutex_lock(&dongle->dongle_mtx);
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
}




void swap_priority(int queue[2])
{
	int temp;

	temp = queue[0];
	queue[0] = queue[1];
	queue[1] = temp;
}


int	check_dongle(t_dongle *dongle, long long request_time, int coder_id)
{
	if (dongle->available == FALSE)
		return (FALSE);
	if (request_time - dongle->last_time_used - dongle->utils_const[TM_COMPILE] < dongle->utils_const[DONGLE_COOLDOWN])
		return (FALSE);
	if (dongle->queue[0] != coder_id)
		return (FALSE);
	return (TRUE);
}
