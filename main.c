/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcondemi <bcondemi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/23 17:42:19 by bcondemi          #+#    #+#             */
/*   Updated: 2026/05/27 17:57:06 by bcondemi         ###   ########.fr       */
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
	if (init_manager(argv, utils_const, coders, manager) == -1)
		return (free_coder_and_manager(coders, manager));
	int j = 0;
	while (j < atoi(argv[1]))
	{
		printf("%d\n", manager->coders[j]->id);
		j++;
	}
	ft_big_free(coders, utils_const, manager);
}


	// int j = 0;
	// while (j < utils_const[NB_CODERS])
	// {
		// printf("elt == %d, left == %d, right == %d\n", coders[j]->id, coders[j]->left->id,coders[j]->right->id);
		// j++;
	// }