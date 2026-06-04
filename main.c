/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcondemi <bcondemi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/23 17:42:19 by bcondemi          #+#    #+#             */
/*   Updated: 2026/06/04 15:22:30 by bcondemi         ###   ########.fr       */
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
	if (monitor(manager) == -1)
	{
		final_clean(manager);
		return (-1);
	}
	final_clean(manager);
	return (0);
}

// to do function protect all mutex
// to protect all mutex of all coders
// to readapt free function and everything to clean dongle mutex
// to do the monitor stuff to handle each coder and time correctly


// to do the makefiel without wildcard at the end

// 	}
// int j = 0;
// while (j < utils_const[NB_CODERS])
// {
// 	printf("elt == %d, queu[0] = %d, queu[1] = %d, priority type == %d\n",
// 		manager->dongles[j].id,
// 		manager->dongles[j].queue[0]->id,
// 		manager->dongles[j].queue[1]->id,
// 		*manager->dongles[j].priority_type);
// 	j++;
// 	}