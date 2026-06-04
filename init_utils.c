/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcondemi <bcondemi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/05 00:27:25 by bcondemi          #+#    #+#             */
/*   Updated: 2026/06/05 00:39:46 by bcondemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

void	init_dongle_queue(t_manager *manager, int i)
{
	if (i == 0)
	{
		manager->dongles[i].queue[0] = &manager->coders[0];
		manager->dongles[i].queue[1] = &manager->coders[1];
	}
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

int	init_manager(char **argv, t_manager *manager)
{
	int	check_init;
	int	nb_coders;

	nb_coders = manager->utils_const[NB_CODERS];
	manager->coders = malloc(sizeof(t_coder) * nb_coders);
	if (manager->coders == NULL)
	{
		free(manager);
		return (allocation_error());
	}
	manager->dongles = malloc(sizeof(t_dongle) * nb_coders);
	if (manager->dongles == NULL)
	{
		free(manager->coders);
		free(manager);
		return (allocation_error());
	}
	check_init = make_init(manager, argv);
	if (check_init == -1)
		return (free_co_do_ma(manager));
	return (0);
}

int	handle_one_coder(t_manager *manager)
{
	printf("0 %d has taken a dongle\n", manager->coders[0].id);
	usleep(manager->utils_const[TM_BURNOUT]);
	printf("%lld %d has burnout\n",
		manager->utils_const[TM_BURNOUT] + 1, manager->coders[0].id);
	destroy_const_mutex(manager);
	pthread_mutex_destroy(&manager->mutex_manager);
	return (0);
}

void	init_utils_coder(t_manager *manager, int index)
{
	manager->coders[index].mutex_manager = &manager->mutex_manager;
	manager->coders[index].manager = manager;
	manager->coders[index].id = index + 1;
	manager->coders[index].last_compile = 1;
	if (manager->coders[index].id % 2 == 1)
		manager->coders[index].last_compile = 0;
	manager->coders[index].compile_cnt = 0;
	manager->coders[index].utils_const = manager->utils_const;
}
