/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_struct.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcondemi <bcondemi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 14:10:55 by bcondemi          #+#    #+#             */
/*   Updated: 2026/06/03 18:59:49 by bcondemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

int	make_init(t_manager *manager, char **argv)
{
	if (pthread_mutex_init(&manager->protect_nb_ready, NULL) != 0)
		return (thread_error());
	if (pthread_mutex_init(&manager->mutex_print, NULL) != 0)
	{
		pthread_mutex_destroy(&manager->protect_nb_ready);
		return (thread_error());
	}
	if (pthread_cond_init(&manager->cond_ready, NULL) != 0)
	{
		pthread_mutex_destroy(&manager->protect_nb_ready);
		pthread_mutex_destroy(&manager->mutex_print);
		return (thread_error());
	}
	if (pthread_cond_init(&manager->routine_wait_start, NULL))
	{
		pthread_cond_destroy(&manager->cond_ready);
		pthread_mutex_destroy(&manager->protect_nb_ready);
		pthread_mutex_destroy(&manager->mutex_print);
		return (thread_error());
	}
	manager->nb_ready = 0;
	manager->thread_error = FALSE;
	manager->coders = manager->coders;
	manager->utils_const = manager->utils_const;
	manager->check_ready = FALSE;
	assign_const(manager, argv);
	loop_on_coder(manager);
	if (init_dongle(manager) == -1)
		return (-1);
	// to handle if only 1 coder to find a solution to problem
	// to ask he to make it burn out
	// to see also if only two element left and right are the same to handle
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


void	init_coder(int index, t_manager *manager)
{
	t_dongle	*norm;

	norm = &manager->dongles[manager->utils_const[NB_CODERS] - 1];
	manager->coders[index].manager = manager;
	manager->coders[index].id = index + 1;
	manager->coders[index].compile_cnt = 0;
	manager->coders[index].utils_const = manager->utils_const;
	manager->coders[index].last_compile = -1;
	if (index == 0)
	{
		manager->coders[index].left = norm;
		manager->coders[index].right = &manager->dongles[index];
	}
	else if (index + 1 == manager->utils_const[NB_CODERS])
	{
		manager->coders[index].left = &manager->dongles[index - 1];
		manager->coders[index].right = &manager->dongles[index];
	}
	else
	{
		manager->coders[index].left = &manager->dongles[index - 1];
		manager->coders[index].right = &manager->dongles[index];
	}
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

int	init_dongle(t_manager *manager)
{
	int	i;

	i = 0;
	while (i < manager->utils_const[NB_CODERS])
	{
		manager->dongles[i].id = i + 1;
		if (pthread_mutex_init(&manager->dongles[i].dongle_mtx, NULL) != 0)
		{
			destroy_const_mutex(manager);
			destroy_mutex_dongle(manager, i);
			return (thread_error());
		}
		manager->dongles[i].last_time_used = 0;
		manager->dongles[i].available = TRUE;
		manager->dongles[i].utils_const = manager->utils_const;
		manager->dongles[i].queue_size = 2;
		init_dongle_queue(manager, i);
		i++;
	}
	return (0);
}

// i = 0;
// while (i < manager->utils_const[NB_CODERS])
// {
	// printf("Dongle %d ;[%d, %d]\n", manager->dongles[i].id, manager->dongles[i].queue[0], manager->dongles[i].queue[1]);
	// i++;
// }

void	init_dongle_queue(t_manager *manager, int i)
{
	if (i == 0)
	{
		manager->dongles[i].queue[0] = 1;
		manager->dongles[i].queue[1] = 2;
	}
	else if (i == manager->utils_const[NB_CODERS] - 1)
	{
		manager->dongles[i].queue[0] = 1;
		manager->dongles[i].queue[1] = i + 1;
	}
	else
	{
		if (i % 2 == 1)
		{
			manager->dongles[i].queue[1] = i + 1;
			manager->dongles[i].queue[0] = i + 2;
		}
		else
		{
			manager->dongles[i].queue[0] = i + 1;
			manager->dongles[i].queue[1] = i + 2;
		}
	}
}

int init_manager(char **argv, t_manager *manager)
{
	int check_init;

	manager->coders = malloc(sizeof(t_coder) * manager->utils_const[NB_CODERS]);
	if (manager->coders == NULL)
	{
		free(manager);
		return (allocation_error());
	}
	manager->dongles = malloc(sizeof(t_dongle) * manager->utils_const[NB_CODERS]);
	if (manager->dongles == NULL)
	{
		free(manager->dongles);
		free(manager);
		return (allocation_error());
	}
	manager->utils_const = manager->utils_const;
	manager->coders = manager->coders;
	manager->dongles = manager->dongles;
	check_init = make_init(manager, argv);
	if (check_init== -1)
	{
		free(manager->dongles);
		free(manager->coders);
		free(manager);
		return (-1);
	}
	return (0);
}
