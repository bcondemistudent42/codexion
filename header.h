/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   header.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcondemi <bcondemi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/23 00:14:28 by bcondemi          #+#    #+#             */
/*   Updated: 2026/05/29 16:02:21 by bcondemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEADER_H
# define HEADER_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <pthread.h>

typedef struct s_coder	t_coder;

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
}	t_const;

typedef enum Bool
{
	FALSE,
	TRUE
}	t_bool;

typedef struct s_dongle
{
	int	id;
	long long last_used;
	t_bool available;
	int	*utils_const;
}	t_dongle;


typedef struct s_Manager
{
	t_coder	**coders;
	t_dongle	*dongles;
	int	*utils_const;
	int	nb_ready;
	pthread_cond_t manager_sleep;
	pthread_mutex_t mutex_manager;
	pthread_cond_t start_cond;
	pthread_mutex_t start_ready_mtx;
}	t_manager;


typedef enum e_scheduler
{
	SCHED_ERROR = -1,
	SCHED_FIFOO = 1,
	SCHED_EDF = 2
}	t_scheduler;


struct	s_coder
{
	int			id;
	int			nb_dongle;
	int			compile_cnt;
	int			*utils_const;
	long long	last_compile;
	pthread_t	thread_id;
	t_dongle	*left;
	t_dongle	*right;
	t_state		state;
	t_manager *manager;
};


int		ft_error(void);
int		allocation_error(void);
int	create_thread(int nb_thread, t_manager *manager);
int		parser_manager(int ac, char **argv);
void	ft_free_coders(t_coder **coders, int index_to_stop);
int free_coder_and_manager(t_coder **coders, t_manager *manager);
int free_coder_and_manager_with_error(t_coder **coders, t_manager *manager);
void ft_big_free(t_manager *manager);
int	make_init(t_manager *manager, char **argv);


#endif
