/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcondemi <bcondemi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/23 17:42:19 by bcondemi          #+#    #+#             */
/*   Updated: 2026/05/26 20:07:11 by bcondemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"


void assign_const(char **argv, int utils_const[]);
t_coder	**assignator_coders(int nb_coder, int utils_const[], t_coder *coders[]);
void	init_coder(t_coder *coder, int id, int utils_const[], t_coder *coders[]);

int	main(int ac, char **argv)
{
	int utils_const[7];
	// to see if needed do this int utils_const[6 * sizeof(t_coder)];
	
	t_coder *coders[atoi(argv[1])];

	if (parser_manager(ac, argv) == -1)
	return (ft_error());

	// to if null return error
	assign_const(argv, utils_const);
	// to if null return error
	assignator_coders(atoi(argv[1]), utils_const, coders);
	int j = 0;
	while (coders[j])
	{
		printf("index == %d, elt == %d\n", j, coders[j]->id);
		j++;
	}
	// while (utils_const[j])
	// {
		// printf("index == %d,value == %d\n\n", j, utils_const[j]);
		// j++;
	// }
	
	
}

void assign_const(char **argv, int utils_const[])
{
	utils_const[NB_CODERS] = atoi(argv[1]);
	utils_const[TM_BURNOUT] = atoi(argv[2]);
	utils_const[TM_COMPILE] = atoi(argv[3]);
	utils_const[TM_DEBUG] = atoi(argv[4]);
	utils_const[TM_REFACTO] = atoi(argv[5]);
	utils_const[COMPILE_REQUIRED] = atoi(argv[6]);
	utils_const[DONGLE_COOLDOWN] = atoi(argv[7]);
}

t_coder	**assignator_coders(int nb_coder, int utils_const[], t_coder *coders[])
{
	int	i;
	t_coder	*my_coder;

	// int *coders to assign each coder in a array index
	i = 0;
	while (i < nb_coder)
	{
		my_coder = malloc(sizeof(t_coder));
		if (my_coder == NULL)
			return (NULL);
		init_coder(my_coder, i, utils_const, coders);
		coders[i] = my_coder;
		i++;
	}
	return (coders);
}


void	init_coder(t_coder *coder, int id, int utils_const[], t_coder *coders[])
{
	coder->id = id;
	coder->nb_dongle = 1;
	coder->compile_cnt = 0;
	coder->utils_const = utils_const;
	coder->	last_compile = -1; //to see which value to init
	if (id == 0)
		
	{
		coder->left = coders[utils_const[NB_CODERS] - 1]; //handle coders if start and end
		coder->right = coders[id + 1]; //handle coders if start and end
	}
	else if (id + 1== utils_const[NB_CODERS]) // to see with -1 or no
	{
		coder->left = coders[id - 1];
		coder->right = coders[0];
	}
	else
	{
		coder->left = coders[id - 1];
		coder->right = coders[id + 1];
	}
	coder->state = INACTIVE;
}
