/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_struct.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcondemi <bcondemi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 14:10:55 by bcondemi          #+#    #+#             */
/*   Updated: 2026/06/03 14:11:38 by bcondemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

int	make_init(t_manager *manager, char **argv)
{
	if (pthread_mutex_init(&manager->protect_nb_ready, NULL) != 0)
		return (ft_big_free_error(manager));
	if (pthread_mutex_init(&manager->mutex_print, NULL) != 0)
	{
		pthread_mutex_destroy(&manager->protect_nb_ready);
		return (ft_big_free_error(manager));
	}
	if (pthread_cond_init(&manager->cond_ready, NULL) != 0)
	{
		pthread_mutex_destroy(&manager->protect_nb_ready);
		pthread_mutex_destroy(&manager->mutex_print);
		return (ft_big_free_error(manager));
	}
	if (pthread_cond_init(&manager->routine_wait_start, NULL))
	{
		pthread_cond_destroy(&manager->cond_ready);
		pthread_mutex_destroy(&manager->protect_nb_ready);
		pthread_mutex_destroy(&manager->mutex_print);
		return (ft_big_free_error(manager));
	}
	manager->nb_ready = 0;
	manager->coders = manager->coders;
	manager->utils_const = manager->utils_const;
	manager->check_ready = FALSE;
	assign_const(manager, argv);
	init_dongle(manager);
	if (assignator_coders(manager) == -1)
		return (ft_big_free_error(manager));
	loop_on_coder(manager);
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
	t_dongle	*norm;

	norm = &manager->dongles[manager->utils_const[NB_CODERS] - 1];
	manager->coders[index]->manager = manager;
	manager->coders[index]->id = index + 1;
	manager->coders[index]->compile_cnt = 0;
	manager->coders[index]->utils_const = manager->utils_const;
	manager->coders[index]->last_compile = -1;
	if (index == 0)
	{
		manager->coders[index]->left = norm;
		manager->coders[index]->right = &manager->dongles[index];
	}
	else if (index + 1 == manager->utils_const[NB_CODERS])
	{
		manager->coders[index]->left = &manager->dongles[index - 1];
		manager->coders[index]->right = &manager->dongles[index];
	}
	else
	{
		manager->coders[index]->left = &manager->dongles[index - 1];
		manager->coders[index]->right = &manager->dongles[index];
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

void	init_dongle(t_manager *manager)
{
	int	i;

	i = 0;
	while (i < manager->utils_const[NB_CODERS])
	{
		manager->dongles[i].id = i + 1;
		pthread_mutex_init(&manager->dongles[i].dongle_mtx, NULL);
		manager->dongles[i].last_time_used = 0;
		manager->dongles[i].available = TRUE;
		manager->dongles[i].utils_const = manager->utils_const;
		manager->dongles[i].queue_size = 2;
		init_dongle_queue(manager, i);
		i++;
	}
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

int init_manager(char **argv, t_manager *manager, t_coder **coders, t_dongle *dongles)
{
	coders = malloc(sizeof(t_coder *) * manager->utils_const[NB_CODERS]);
	if (coders == NULL)
	{
		free(manager);
		return (allocation_error());
	}
	dongles = malloc(sizeof(t_dongle) * manager->utils_const[NB_CODERS]);
	if (dongles == NULL)
		return (free_coder_and_manager(coders, manager));
	manager->utils_const = manager->utils_const;
	manager->coders = coders;
	manager->dongles = dongles;
	if (make_init(manager, argv) == -1)
	{
		// free(coders);
		// free(dongles);
		return (-1);
	}
	return (0);
}
