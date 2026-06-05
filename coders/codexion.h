/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcondemi <bcondemi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/23 00:14:28 by bcondemi          #+#    #+#             */
/*   Updated: 2026/06/05 13:46:57 by bcondemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <sys/time.h>
# include <pthread.h>

typedef struct s_coder	t_coder;

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

typedef enum Queu_Type
{
	EDF,
	FIFO
}	t_queu_type;

typedef struct s_dongle
{
	int				id;
	int				*priority_type;
	t_bool			available;
	long long		last_time_used;
	long long		*utils_const;
	t_coder			*queue[2];
	pthread_mutex_t	dongle_mtx;
}	t_dongle;

typedef struct s_Manager
{
	int				nb_ready;
	int				end_type;
	int				thread_error;
	int				priority_type;
	int				check_ready;
	t_coder			*coders;
	t_dongle		*dongles;
	long long		*utils_const;
	pthread_mutex_t	protect_nb_ready;
	pthread_mutex_t	mutex_print;
	pthread_mutex_t	mutex_manager;
	pthread_cond_t	cond_ready;
	pthread_cond_t	routine_wait_start;
	pthread_t		manager_thread;

}	t_manager;

typedef enum e_utils
{
	EQUAL_BURNOUT = -1,
	BURNOUT_ERROR,
	RUNNING,
	FINISHED
}	t_utils;

typedef enum e_scheduler
{
	SCHED_ERROR = -1,
	SCHED_FIFOO = 1,
	SCHED_EDF = 2,
}	t_scheduler;

struct	s_coder
{
	int				id;
	int				compile_cnt;
	long long		last_compile;
	t_dongle		*left;
	t_dongle		*right;
	t_manager		*manager;
	pthread_t		thread_id;
	pthread_mutex_t	coder_mutex;
	pthread_mutex_t	*mutex_manager;
	long long		*utils_const;
};

// INIT
int				big_check_mutex(t_manager *manager);
int				init_dongle(t_manager *manager);
int				loop_on_coder(t_manager *manager);
int				parser_manager(int ac, char **argv);
int				init_coder(int index, t_manager *manager);
int				make_init(t_manager *manager, char **argv);
int				init_manager(char **argv, t_manager *manager);
void			init_dongle_queue(t_manager *manager, int i);
void			init_utils_coder(t_manager *manager, int index);
void			assign_const(t_manager *manager, char **argv);

// THREAD UTILS
int				coders_actions(t_coder *coder);
int				unlock_and_release(t_coder *coder);
int				check_burnout_happen(t_coder *coder);
int				can_compile(t_coder *coder);
int				create_thread(t_manager *manager);
int				make_thread_join(t_manager *manager, int index);
int				check_dongle(t_dongle *dongle, int coder_id);
void			launch_thread(t_manager *manager);
void			my_function(void *manager);
void			wait_for_start(t_coder *coder);
void			wait_for_start_manager(t_manager *manager);

void			update_compile_count(t_coder *coder);
void			take_both_dongle(t_coder *coder);
void			release_both_dongle(t_coder *coder);
void			take_dongle(t_dongle *dongle, t_coder *coder);
void			release_dongle(t_dongle *dongle, long long time_release);
void			swap_priority(t_coder *queue[2]);

// MONITOR
int				handle_one_coder(t_manager *manager);
int				handle_burnout(t_dongle *dongle);
int				monitor(t_manager *manager);
int				compile(t_coder *coder);
int				debug(t_coder *coder);
int				refacto(t_coder *coder);
int				check_burnout(t_coder *coder);
int				check_burnout_all_coders(t_manager *manager);
int				litlle_burnout(t_dongle *dongle);
int				find_closest_burnout(t_dongle *dongle);
int				check_finish(t_manager *manager);
int				end_type_handler(t_manager *manager);
void			monitor_checker(void *the_manager);
void			start_timestamp(void);
long long		get_time(void);

// CLEAN
int				free_co_do_ma(t_manager *manager);
int				custom_clean(t_manager *manager);
int				ft_error(void);
int				allocation_error(void);
int				thread_error(void);
void			final_clean(t_manager *manager);
void			one_coder_clean(t_manager *manager);
void			destroy_const_mutex(t_manager *manager);
void			destroy_mutex_dongle(t_manager *manager, int max);
void			destroy_mutex_coders(t_manager *manager, int max);

#endif