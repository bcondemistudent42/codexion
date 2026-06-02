/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_struct.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcondemi <bcondemi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 14:10:55 by bcondemi          #+#    #+#             */
/*   Updated: 2026/06/02 11:02:45 by bcondemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

void	assign_const(t_manager *manager, char **argv);
int	assignator_coders(t_manager *manager);
void	loop_on_coder(t_manager *manager);
void	init_coder(int index, t_manager *manager);
void init_dongle(t_manager *manager);

int	make_init(t_manager *manager, char **argv)
{
	pthread_mutex_init(&manager->protect_nb_ready, NULL); // to destroy at end
	pthread_mutex_init(&manager->mutex_print, NULL); // to destroy at end
	pthread_cond_init(&manager->cond_ready, NULL); // to destroy at end
	pthread_cond_init(&manager->routine_wait_start, NULL); // to destroy at end
	
	manager->nb_ready = 0;
	manager->coders = manager->coders;
	manager->utils_const = manager->utils_const;
	manager->check_ready = FALSE;
	
	assign_const(manager, argv);
	init_dongle(manager);
	if (assignator_coders(manager) == -1)
		return (-1);
	loop_on_coder(manager);
	// to handle if only 1 coder to find a solution to problem
	// to ask he to make it burn out
	// to see also if only two element left and right are the same to handle
	// pthread_mutex_init(&manager->start_ready_mtx, NULL); // to destroy at end
	// pthread_mutex_init(&manager->var_mutex, NULL); // to destroy at end
		// pthread_mutex_init(&manager->dongle_mutex, NULL);
	// pthread_mutex_init(&manager->print_mutex, NULL);
	return (0);
}

void	assign_const(t_manager *manager, char **argv)
{
	manager->utils_const[NB_CODERS] = atoi(argv[1]);
	manager->utils_const[TM_BURNOUT] = atoi(argv[2]);
	manager->utils_const[TM_COMPILE] = atoi(argv[3]);
	manager->utils_const[TM_DEBUG] = atoi(argv[4]);
	manager->utils_const[TM_REFACTO] = atoi(argv[5]);
	manager->utils_const[COMPILE_REQUIRED] = atoi(argv[6]);
	manager->utils_const[DONGLE_COOLDOWN] = atoi(argv[7]);
}

int	assignator_coders(t_manager *manager)
{
	int		i;
	t_coder	*my_coder;

	i = 0;
	while (i < manager->utils_const[NB_CODERS])
	{
		my_coder = malloc(sizeof(t_coder));
		if (my_coder == NULL)
		{
			free(manager->dongles);
			ft_free_coders(manager->coders, i);
			return (-1);
		}
		manager->coders[i] = my_coder;
		i++;
	}
	return (0);
}

void	init_coder(int index, t_manager *manager)
{
	manager->coders[index]->manager = manager;
	manager->coders[index]->id = index + 1;
	manager->coders[index]->nb_dongle = 1;
	manager->coders[index]->compile_cnt = 0;
	manager->coders[index]->utils_const = manager->utils_const;
	manager->coders[index]->last_compile = 0;
	if (index == 0)
	{
		manager->coders[index]->left = &manager->dongles[manager->utils_const[NB_CODERS] - 1];
		manager->coders[index]->right = &manager->dongles[index + 1];
	}
	else if (index + 1 >= manager->utils_const[NB_CODERS])
	{
		manager->coders[index]->left = &manager->dongles[index - 1];
		manager->coders[index]->right = &manager->dongles[0];
	}
	else
	{
		manager->coders[index]->left = &manager->dongles[index - 1];
		manager->coders[index]->right = &manager->dongles[index + 1];
	}
	manager->coders[index]->state = INACTIVE;
}

void	loop_on_coder(t_manager *manager)
{
	int	i;

	i = 0;
	while (i < manager->utils_const[NB_CODERS])
	{
		init_coder(i, manager);
		i++;
	}
}

void init_dongle(t_manager *manager)
{
	int  i;

	i = 0;
	while (i < manager->utils_const[NB_CODERS])
	{
		manager->dongles[i].id = i + 1;
		manager->dongles[i].last_time_used = 0; //to see if -1 is good value
		manager->dongles[i].available = TRUE;
		manager->dongles[i].utils_const = manager->utils_const;
		manager->dongles[i].queue_size = 0;
		i++;
	}
}