/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   header.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcondemi <bcondemi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/23 00:14:28 by bcondemi          #+#    #+#             */
/*   Updated: 2026/06/02 15:10:45 by bcondemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEADER_H
# define HEADER_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <sys/time.h>
# include <pthread.h>

typedef struct s_coder	t_coder;

// typedef struct	s_hp_elt
// {
	// int			id;
	// long long	last_compile;
// }	t_hp_elt; to which solution to choose


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
	TM_START,
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
	long long last_time_used;
	int queue[2];
	int queue_size;
	t_bool available;
	pthread_mutex_t dongle_mtx;
	long long	*utils_const;
}	t_dongle;


typedef struct s_Manager
{
	t_coder	**coders;
	t_dongle	*dongles;
	long long	*utils_const;
	int	nb_ready;
	int check_ready;
	pthread_mutex_t protect_nb_ready;
	pthread_mutex_t mutex_print;
	pthread_cond_t cond_ready;
	pthread_cond_t routine_wait_start;

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
	int			compile_cnt;
	long long			*utils_const;
	long long	last_compile;
	pthread_t	thread_id;
	t_dongle	*left;
	t_dongle	*right;
	t_state		state;
	t_manager *manager;
};


int		ft_error(void);
long long get_time();
void ft_set_coders_time(t_manager *manager);
int		allocation_error(void);
int	create_thread(t_manager *manager);
int		parser_manager(int ac, char **argv);
void	ft_free_coders(t_coder **coders, int index_to_stop);
int free_coder_and_manager(t_coder **coders, t_manager *manager);
int free_coder_and_manager_with_error(t_coder **coders, t_manager *manager);
void ft_big_free(t_manager *manager);
int	make_init(t_manager *manager, char **argv);


#endif
