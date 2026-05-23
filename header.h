/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   header.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcondemi <bcondemi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/23 00:14:28 by bcondemi          #+#    #+#             */
/*   Updated: 2026/05/23 17:42:39 by bcondemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef HEADER_H
#define HEADER_H


# include <stdio.h>
#include <stdlib.h>
#include <string.h>


int ft_error();
int	parser_manager(int ac, char **argv);

typedef enum e_scheduler
{
	SCHED_ERROR = -1,
	SCHED_FIFO = 1,
	SCHED_EDF = 2
}	t_scheduler;


// struct manager{
// 	int number_worker;
// 	int number_dongles;
// } s_manager;


#endif






