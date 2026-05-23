/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcondemi <bcondemi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/22 23:52:51 by bcondemi          #+#    #+#             */
/*   Updated: 2026/05/23 17:44:16 by bcondemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "header.h"

int check_number(int n);
int check_nb_worker(int n);
int check_scheduler(char *sring);


int	parser_manager(int ac, char **argv)
{
	int	i;

	i = 2;
	if (ac != 9)
		return (-1);
	if (check_nb_worker(atoi(argv[1])) == -1)
		return (-1);
		while (i < 8)
		if (check_number(atoi(argv[i])) == -1)
			return (-1);
		else
			i++;
	if (check_scheduler(argv[8]) == SCHED_ERROR)
		return (-1);
	return (1);
}


int ft_error()
{
	printf("Error Usage:\n");
	printf("int: number_of_coders > 0\n");
	printf("int: time_to_burnout\n");
	printf("int: time_to_compile\n");
	printf("int: time_to_debug\n");
	printf("int: time_to_refactor\n");
	printf("int: number_of_compiles_required\n");
	printf("int: dongle_cooldown\n");
	printf("scheduler: {'fifo' or 'edf'}\n");
	return (-1);
}


int check_number(int n)
{
	if (n < 0 ||
		n > 2147483647)
		return (-1);
	return (1);
}


int check_nb_worker(int n)
{
	if (n <= 0 ||
		n > 2147483647)
		return (-1);
	return (1);
}


int check_scheduler(char *string)
{
	int fifo;
	int edf;

	fifo = strcmp(string, "fifo");
	edf = strcmp(string, "edf");
	
	if (fifo != 0 && edf != 0)
		return (SCHED_ERROR);
	else if (fifo == 0)
		return (SCHED_FIFO);
	return (SCHED_EDF);
}
