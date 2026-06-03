/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   header.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcondemi <bcondemi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/23 00:14:28 by bcondemi          #+#    #+#             */
/*   Updated: 2026/06/03 17:19:29 by bcondemi         ###   ########.fr       */
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
	int				id;
	int				queue[2];
	int				queue_size;
	t_bool			available;
	long long		last_time_used;
	long long		*utils_const;
	pthread_mutex_t	dongle_mtx;
}	t_dongle;

typedef struct s_Manager
{
	int				nb_ready;
	int				check_ready;
	t_coder			*coders;
	t_dongle		*dongles;
	long long		*utils_const;
	pthread_mutex_t	protect_nb_ready;
	pthread_mutex_t	mutex_print;
	pthread_cond_t	cond_ready;
	pthread_cond_t	routine_wait_start;

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
	long long	last_compile;
	t_dongle	*left;
	t_dongle	*right;
	// t_state		state;
	t_manager	*manager;
	pthread_t	thread_id;
	long long	*utils_const;
};

// Init Struct
void	assign_const(t_manager *manager, char **argv);
void	init_dongle_queue(t_manager *manager, int i);
int	init_dongle(t_manager *manager);
void	loop_on_coder(t_manager *manager);
void	init_coder(int index, t_manager *manager);
int		assignator_coders(t_manager *manager);
int		make_init(t_manager *manager, char **argv);
int		init_manager(char **argv, t_manager *manager);

// Clean
void final_clean(t_manager *manager);
void destroy_const_mutex(t_manager *manager);
int		ft_error(void);
int		allocation_error(void);
int	thread_error(void);
void destroy_mutex_dongle(t_manager *manager, int max);

// Thread Utils
void	my_function(void *manager);
void	swap_priority(int queue[2]);
void	release_both_dongle(t_coder *coder);
void	launch_thread(t_manager *manager);
void	wait_for_start(t_coder *coder);
void	take_both_dongle(t_coder *coder);
void	take_dongle(t_dongle *dongle, t_coder *coder);
void	release_dongle(t_dongle *dongle, long long time_release);
int		can_compile(t_coder *coder);
int		can_compile(t_coder *coder);
int		create_thread(t_manager *manager);
int		make_thread_join(t_manager *manager, int index);
int		check_dongle(t_dongle *dongle, long long request_time, int coder_id);

// other to see
long long	get_time();
int		parser_manager(int ac, char **argv);

#endif
