/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_struct.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcondemi <bcondemi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 14:10:55 by bcondemi          #+#    #+#             */
/*   Updated: 2026/06/05 18:58:30 by bcondemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	make_init(t_manager *manager, char **argv)
{
	int	mutex_check;

	mutex_check = big_check_mutex(manager);
	if (mutex_check != 0)
		return (-1);
	assign_const(manager, argv);
	if (loop_on_coder(manager) == -1)
	{
		destroy_const_mutex(manager);
		pthread_mutex_destroy(&manager->mutex_manager);
		return (-1);
	}
	if (init_dongle(manager) == -1)
	{
		destroy_const_mutex(manager);
		pthread_mutex_destroy(&manager->mutex_manager);
		return (-1);
	}
	if (manager->utils_const[NB_CODERS] == 1)
		return (handle_one_coder(manager));
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

int	init_coder(int index, t_manager *manager)
{
	t_dongle	*norm;

	norm = &manager->dongles[manager->utils_const[NB_CODERS] - 1];
	init_utils_coder(manager, index);
	if (pthread_mutex_init(&manager->coders[index].coder_mutex, NULL) != 0)
	{
		destroy_mutex_coders(manager, index);
		return (thread_error());
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
			destroy_mutex_dongle(manager, i);
			destroy_mutex_coders(manager, manager->utils_const[NB_CODERS]);
			return (thread_error());
		}
		manager->dongles[i].available = TRUE;
		manager->dongles[i].last_time_used = -manager->utils_const[DONGLE_COOLDOWN];
		manager->dongles[i].priority_type = &manager->priority_type;
		manager->dongles[i].utils_const = manager->utils_const;
		init_dongle_queue(manager, i);
		i++;
	}
	return (0);
}
