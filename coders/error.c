/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcondemi <bcondemi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 14:17:16 by bcondemi          #+#    #+#             */
/*   Updated: 2026/06/09 13:59:46 by bcondemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

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

int	allocation_error(void)
{
	printf("A malloc failed allocating memory\n");
	return (-1);
}

int	thread_error(void)
{
	printf("A pthread Error occured\n");
	return (-1);
}

int	too_much(void)
{
	printf("Max Coders number is 200\n");
	return (-1);
}
