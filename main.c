/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcondemi <bcondemi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/23 17:42:19 by bcondemi          #+#    #+#             */
/*   Updated: 2026/05/27 14:38:57 by bcondemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

int	main(int ac, char **argv)
{
	int utils_const[7];
	t_coder *coders[atoi(argv[ac % 2])];

	if (parser_manager(ac, argv) == -1)
	return (ft_error());

	if (init_manager(argv, utils_const, coders) == -1)
		return (allocation_error());
	int j = 0;
	while (j < utils_const[NB_CODERS])
	{
		printf("elt == %d, left == %d, right == %d\n", coders[j]->id, coders[j]->left->id,coders[j]->right->id);
		j++;
	}
}
