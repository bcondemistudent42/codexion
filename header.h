/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   header.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcondemi <bcondemi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/23 00:14:28 by bcondemi          #+#    #+#             */
/*   Updated: 2026/05/26 19:56:46 by bcondemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEADER_H
# define HEADER_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>

int	ft_error(void);
int	parser_manager(int ac, char **argv);


typedef enum State
{
	COMPILING,
	DEBUG,
	REFACTO,
	INACTIVE
}	t_state;


typedef enum Const
{
	TM_BURNOUT,
	TM_COMPILE,
	TM_DEBUG,
	TM_REFACTO,
	COMPILE_REQUIRED,
	DONGLE_COOLDOWN,
	NB_CODERS
}	t_Const;

typedef struct	s_Manager
{
	int	*coders;
	int	*utils_const;
}	t_manager;


typedef struct s_coder	t_coder;
struct	s_coder
{
	int			id;
	int			nb_dongle;
	int			compile_cnt;
	int			*utils_const;
	long long	last_compile;
	t_coder		*left;
	t_coder		*right;
	t_state		state;
};

typedef enum e_scheduler
{
	SCHED_ERROR = -1,
	SCHED_FIFO = 1,
	SCHED_EDF = 2
}	t_scheduler;

#endif
