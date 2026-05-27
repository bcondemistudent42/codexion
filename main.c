/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcondemi <bcondemi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/23 17:42:19 by bcondemi          #+#    #+#             */
/*   Updated: 2026/05/27 14:07:23 by bcondemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"


void assign_const(char **argv, int utils_const[]);
int	assignator_coders(int utils_const[], t_coder *coders[]);
void	loop_on_coder(int utils_const[], t_coder *coders[]);
void	init_coder(int index, int utils_const[], t_coder *coders[]);

int	main(int ac, char **argv)
{
	int utils_const[7];
	t_coder *coders[atoi(argv[1])];

	if (parser_manager(ac, argv) == -1)
	return (ft_error());

	assign_const(argv, utils_const);
	// to if null return error
	assignator_coders(utils_const, coders);
	loop_on_coder(utils_const, coders);
	int j = 0;
	while (j < atoi(argv[1]))
	{
		printf("elt == %d, left == %d, right == %d\n", coders[j]->id, coders[j]->left->id,coders[j]->right->id);
		j++;
	}
	
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

int	assignator_coders(int utils_const[], t_coder *coders[])
{
	int	i;
	t_coder	*my_coder;

	// int *coders to assign each coder in a array index
	i = 0;
	while (i < utils_const[NB_CODERS])
	{
		my_coder = malloc(sizeof(t_coder));
		if (my_coder == NULL)
		// to free all element before return
			return (-1);
		coders[i] = my_coder;
		i++;
	}
	return (0);
}


void	init_coder(int index, int utils_const[], t_coder *coders[])
{
	int id;

	id = index + 1;
	coders[index]->id = id;
	coders[index]->nb_dongle = 1;
	coders[index]->compile_cnt = 0;
	coders[index]->utils_const = utils_const;
	coders[index]->	last_compile = -1; //to see which value to init
	if (index == 0)
	{
		coders[index]->left = coders[utils_const[NB_CODERS] - 1]; //handle coders if start and end
		coders[index]->right = coders[index + 1]; //handle coders if start and end
	}
	else if (index + 1 >= utils_const[NB_CODERS]) // to see with -1 or no
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


void	loop_on_coder(int utils_const[], t_coder *coders[])
{
	int	i;

	i = 0;
	while (i < utils_const[NB_CODERS])
	{
		init_coder(i, utils_const, coders);
		i++;
	}
}