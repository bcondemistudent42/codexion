/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcondemi <bcondemi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/23 17:42:19 by bcondemi          #+#    #+#             */
/*   Updated: 2026/05/28 18:40:15 by bcondemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

int	main(int ac, char **argv)
{
	int		utils_const[7];
	t_coder	**coders;
	t_manager *manager;

	if (parser_manager(ac, argv) == -1)
		return (ft_error());
	coders = malloc(sizeof(t_coder) * atoi(argv[1]));
	if (coders == NULL)
		return (allocation_error());
	manager = malloc(sizeof(t_manager));
	if (manager == NULL)
		return (free_coder_and_manager(coders, manager));
	if (make_init_coders(argv, utils_const, coders, manager) == -1)
		return (free_coder_and_manager(coders, manager));

	create_thread(utils_const[NB_CODERS], manager);
	ft_big_free(coders, utils_const, manager);
}

// to link dongles to coders
// to do create thread and my_function work correctly
// to do the monitor stuff to handle each coder and time correctly
// to clean all mutexes after uses


	// int j = 0;
	// while (j < utils_const[NB_CODERS])
	// {
		// printf("elt == %d, left == %d, right == %d\n", coders[j]->id, coders[j]->left->id,coders[j]->right->id);
		// j++;
	// }