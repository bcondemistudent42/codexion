/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_thread.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcondemi <bcondemi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 15:53:03 by bcondemi          #+#    #+#             */
/*   Updated: 2026/05/27 19:11:10 by bcondemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"


int create_thread(int nb_thread, t_manager *manager)
{
	int	i;
	pthread_t my_thread;
	// pthread_cond_t my_cond;
	// pthread_cond_init(&my_cond);

	i = 0;
	while (i < nb_thread)
	{
		pthread_create(&my_thread, NULL, my_function, (void *)manager); // to protect
		i++;
	}
	// tant que simulation pas start et moniteur not created and threads not created wait
}


int my_function(void *manager)
{
	t_manager *handler;
	pthread_mutex_t mutex;
	
	handler = (t_manager *)manager;
	pthread_cond_wait(to be all ready);
	pthread_mutex_lock(&mutex);
	// checking if dongle available 
	// compile 
	// debug 
	// refacto
	pthread_mutex_unlock(&mutex);

}