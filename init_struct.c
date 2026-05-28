/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_struct.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcondemi <bcondemi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 14:10:55 by bcondemi          #+#    #+#             */
/*   Updated: 2026/05/28 18:13:33 by bcondemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

void	assign_const(char **argv, int utils_const[]);
int	assignator_coders(int utils_const[], t_coder **coders);
void	loop_on_coder(int utils_const[], t_coder **coders, t_manager *manager);
void	init_coder(int index, int utils_const[], t_coder **coders, t_manager *manager);

int	make_init_coders(char **argv, int utils_const[], t_coder **coders, t_manager *manager)
{
	assign_const(argv, utils_const);
	// to handle if only 1 coder to find a solution to problem
	// to ask he to make it burn out
	// to see also if only two element left and right are the same to handle
	manager->nb_ready = 0;
	pthread_cond_init(&manager->manager_sleep, NULL); // to destroy at end
	pthread_cond_init(&manager->start_cond, NULL); // to destroy at end
	pthread_mutex_init(&manager->mutex_manager, NULL); // to destroy at end
	pthread_mutex_init(&manager->start_ready_mtx, NULL); // to destroy at end
	if (assignator_coders(utils_const, coders) == -1)
		return (-1);
	loop_on_coder(utils_const, coders, manager);
	manager->coders = coders;
	manager->utils_const = utils_const;
	return (0);
}

void	assign_const(char **argv, int utils_const[])
{
	utils_const[NB_CODERS] = atoi(argv[1]);
	utils_const[TM_BURNOUT] = atoi(argv[2]);
	utils_const[TM_COMPILE] = atoi(argv[3]);
	utils_const[TM_DEBUG] = atoi(argv[4]);
	utils_const[TM_REFACTO] = atoi(argv[5]);
	utils_const[COMPILE_REQUIRED] = atoi(argv[6]);
	utils_const[DONGLE_COOLDOWN] = atoi(argv[7]);
}

int	assignator_coders(int utils_const[], t_coder **coders)
{
	int		i;
	t_coder	*my_coder;

	i = 0;
	while (i < utils_const[NB_CODERS])
	{
		my_coder = malloc(sizeof(t_coder));
		if (my_coder == NULL)
		{
			ft_free_coders(coders, i);
			return (-1);
		}
		coders[i] = my_coder;
		i++;
	}
	return (0);
}

void	init_coder(int index, int utils_const[], t_coder **coders, t_manager *manager)
{
	coders[index]->manager = manager;
	coders[index]->id = index + 1;
	coders[index]->nb_dongle = 1;
	coders[index]->compile_cnt = 0;
	coders[index]->utils_const = utils_const;
	coders[index]->last_compile = -1; //to see which value to init
	if (index == 0)
	{
		coders[index]->left = coders[utils_const[NB_CODERS] - 1];
		coders[index]->right = coders[index + 1];
	}
	else if (index + 1 >= utils_const[NB_CODERS])
	{
		coders[index]->left = coders[index - 1];
		coders[index]->right = coders[0];
	}
	else
	{
		coders[index]->left = coders[index - 1];
		coders[index]->right = coders[index + 1];
	}
	coders[index]->state = INACTIVE;
}

void	loop_on_coder(int utils_const[], t_coder **coders, t_manager *manager)
{
	int	i;

	i = 0;
	while (i < utils_const[NB_CODERS])
	{
		init_coder(i, utils_const, coders, manager);
		i++;
	}
}
