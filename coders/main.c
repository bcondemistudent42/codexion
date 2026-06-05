/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcondemi <bcondemi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/23 17:42:19 by bcondemi          #+#    #+#             */
/*   Updated: 2026/06/06 00:14:14 by bcondemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	main(int ac, char **argv)
{
	long long		utils_const[7];
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
	if (manager->utils_const[NB_CODERS] == 1)
	{
		one_coder_clean(manager);
		return (0);
	}
	if (monitor(manager) == -1)
	{
		final_clean(manager);
		return (-1);
	}
	final_clean(manager);
	return (0);
}

// to check ./codexion 100 105 50 2 1 5 1 edf
// sometimes crashing on pc