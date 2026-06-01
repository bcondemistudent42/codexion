/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcondemi <bcondemi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/23 17:42:19 by bcondemi          #+#    #+#             */
/*   Updated: 2026/06/01 11:31:09 by bcondemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

int	main(int ac, char **argv)
{
	int		utils_const[7];
	t_coder	**coders;
	t_dongle *dongles;
	t_manager *manager;

	if (parser_manager(ac, argv) == -1)
		return (ft_error());
	coders = malloc(sizeof(t_coder *) * atoi(argv[1]));
	if (coders == NULL)
		return (allocation_error());
	manager = malloc(sizeof(t_manager));
	if (manager == NULL)
		return (free_coder_and_manager(coders, manager));
	dongles = malloc(sizeof(t_dongle) * atoi(argv[1]));
	if (dongles == NULL)
		return(free_coder_and_manager(coders, manager));
	manager->utils_const = utils_const;
	manager->coders = coders;
	manager->dongles = dongles;
	if (make_init(manager, argv) == -1)
		return (free_coder_and_manager(coders, manager));

	create_thread(manager);
	int j = 0;
	printf("Time == %lld\n", manager->time_start);
	while (j < utils_const[NB_CODERS])
	{
		printf("elt == %d, left == %d, right == %d, time == %lld\n", coders[j]->id, coders[j]->left->id,coders[j]->right->id, coders[j]->last_compile);
		j++;
	}
	ft_big_free(manager);
	}

// to do the monitor stuff to handle each coder and time correctly
// to clean all mutexes after uses

