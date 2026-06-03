/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcondemi <bcondemi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/23 17:42:19 by bcondemi          #+#    #+#             */
/*   Updated: 2026/06/03 17:19:39 by bcondemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

int	main(int ac, char **argv)
{
	long long		utils_const[8];
	t_manager		*manager;

	if (parser_manager(ac, argv) == -1)
		return (ft_error());
	manager = malloc(sizeof(t_manager));
	if (manager == NULL)
		return (allocation_error());
	manager->utils_const = utils_const;
	manager->utils_const[NB_CODERS] = atoi(argv[1]);
	if (init_manager(argv, manager) == -1)
		return (-1);
	create_thread(manager);
	final_clean(manager);
	// to return -1 if error in create_thread

	return (0);
}


	// to do the monitor stuff to handle each coder and time correctly
	// to clean all mutexes after uses

	// int j = 0;
	// printf("Time == %lld\n", manager->time_start);
	// while (j < utils_const[NB_CODERS])
	// {
	// printf("elt == %d, left == %d, right == %d, time == %lld\n", coders[j]->id, coders[j]->left->id,coders[j]->right->id, coders[j]->last_compile);
	// j++;
// }

// to protect printf ask antoine
// to do hte makefiel without wildcard
// comprendre pourquoi jai du retard par rapport a adrien environ 100ms