/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_struct.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcondemi <bcondemi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 14:10:55 by bcondemi          #+#    #+#             */
/*   Updated: 2026/06/04 15:22:54 by bcondemi         ###   ########.fr       */
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
	assign_const(manager, argv);
	if (loop_on_coder(manager) == -1)
		return (-1);
	if (init_dongle(manager) == -1)
		return (-1);
	// to handle if only 1 coder to find a solution to problem
	// to ask he to make it burn out
	// to see also if only two element left and right are the same to handle
	return (0);
}

void	assign_const(t_manager *manager, char **argv)
{
	manager->nb_ready = 0;
	manager->thread_error = FALSE;
	manager->check_ready = FALSE;
	manager->end_type = RUNNING;
	if (strcmp(argv[8], "fifo") == 0)
		manager->priority_type = FIFO;
	else
		manager->priority_type = EDF;
	manager->utils_const[NB_CODERS] = atoi(argv[1]);
	manager->utils_const[TM_BURNOUT] = atoi(argv[2]);
	manager->utils_const[TM_COMPILE] = atoi(argv[3]);
	manager->utils_const[TM_DEBUG] = atoi(argv[4]);
	manager->utils_const[TM_REFACTO] = atoi(argv[5]);
	manager->utils_const[COMPILE_REQUIRED] = atoi(argv[6]);
	manager->utils_const[DONGLE_COOLDOWN] = atoi(argv[7]);
}

// to change into int to return error if mutex init error
int	init_coder(int index, t_manager *manager)
{
	t_dongle	*norm;

	norm = &manager->dongles[manager->utils_const[NB_CODERS] - 1];
	manager->coders[index].manager = manager;
	manager->coders[index].id = index + 1;
	manager->coders[index].last_compile = -1;
	if (manager->coders[index].id % 2 == 1)
		manager->coders[index].last_compile = -2;
	manager->coders[index].compile_cnt = 0;
	manager->coders[index].utils_const = manager->utils_const;
	// to protect mutex correctly already have done function free coder mutex
	if (pthread_mutex_init(&manager->coders[index].coder_mutex, NULL) != 0)
	{
		destroy_const_mutex(manager);
		destroy_mutex_coders(manager, index);
		return (-1);
	}
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
	return (0);
}

int	loop_on_coder(t_manager *manager)
{
	int	i;

	i = 0;
	while (i < manager->utils_const[NB_CODERS])
	{
		if (init_coder(i, manager) == -1)
			return (-1);
		i++;
	}
	return (0);
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
			destroy_mutex_coders(manager, manager->utils_const[NB_CODERS]);
			return (thread_error());
		}
		manager->dongles[i].available = TRUE;
		manager->dongles[i].last_time_used = 0;
		manager->dongles[i].priority_type = &manager->priority_type;
		manager->dongles[i].utils_const = manager->utils_const;
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
		manager->dongles[i].queue[0] = &manager->coders[0];
		manager->dongles[i].queue[1] = &manager->coders[1]; //to handle when 1 coders make burnout
	} // carreful here danger of SEGFAULT
	else if (i == manager->utils_const[NB_CODERS] - 1)
	{
		manager->dongles[i].queue[0] = &manager->coders[0];
		manager->dongles[i].queue[1] = &manager->coders[i];
	}
	else
	{
		if (i % 2 == 1)
		{
			manager->dongles[i].queue[1] = &manager->coders[i];
			manager->dongles[i].queue[0] = &manager->coders[i + 1];
		}
		else
		{
			manager->dongles[i].queue[0] = &manager->coders[i];
			manager->dongles[i].queue[1] = &manager->coders[i + 1];
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
		free(manager->coders);
		free(manager);
		return (allocation_error());
	}
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
