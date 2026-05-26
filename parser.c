/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcondemi <bcondemi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/22 23:52:51 by bcondemi          #+#    #+#             */
/*   Updated: 2026/05/26 13:11:36 by bcondemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

int	check_number(int n);
int	check_format(char *string);
int	check_scheduler(char *string);

int	parser_manager(int ac, char **argv)
{
	int	i;

	i = 1;
	if (ac != 9)
		return (-1);
	while (i < 8)
	{
		if (check_format(argv[i]) == -1)
			return (-1);
		if (check_number(atoi(argv[i])) == -1)
			return (-1);
		i++;
	}
	if (check_scheduler(argv[8]) == SCHED_ERROR)
		return (-1);
	return (1);
}

int	ft_error(void)
{
	printf("Error Usage:\n");
	printf("int: number_of_coders\n");
	printf("int: time_to_burnout\n");
	printf("int: time_to_compile\n");
	printf("int: time_to_debug\n");
	printf("int: time_to_refactor\n");
	printf("int: number_of_compiles_required\n");
	printf("int: dongle_cooldown\n");
	printf("scheduler: {'fifo' or 'edf'}\n");
	return (-1);
}

int	check_number(int n)
{
	if (n <= 0 || n > 2147483647)
		return (-1);
	return (1);
}

int	check_scheduler(char *string)
{
	int	fifo;
	int	edf;

	fifo = strcmp(string, "fifo");
	edf = strcmp(string, "edf");
	if (fifo != 0 && edf != 0)
		return (SCHED_ERROR);
	else if (fifo == 0)
		return (SCHED_FIFO);
	return (SCHED_EDF);
}

int	check_format(char *string)
{
	int	i;

	i = 0;
	while (string[i])
	{
		if (string[i] < '0' || string[i] > '9')
			return (-1);
		i++;
	}
	return (0);
}
