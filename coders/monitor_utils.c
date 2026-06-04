/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcondemi <bcondemi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/05 00:32:35 by bcondemi          #+#    #+#             */
/*   Updated: 2026/06/05 00:33:03 by bcondemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

int	end_type_handler(t_manager *manager)
{
	if (check_burnout_all_coders(manager) == TRUE)
		return (TRUE);
	else if (check_finish(manager) == TRUE)
		return (TRUE);
	return (FALSE);
}

int	check_finish(t_manager *manager)
{
	int	i;
	int	cnt;

	i = 0;
	cnt = 0;
	while (i < manager->utils_const[NB_CODERS])
	{
		pthread_mutex_lock(&manager->coders[i].coder_mutex);
		if (manager->coders[i].compile_cnt
			>= manager->utils_const[COMPILE_REQUIRED])
			cnt++;
		pthread_mutex_unlock(&manager->coders[i].coder_mutex);
		i++;
	}
	if (cnt == manager->utils_const[NB_CODERS])
	{
		pthread_mutex_lock(&manager->mutex_manager);
		manager->end_type = FINISHED;
		pthread_mutex_unlock(&manager->mutex_manager);
		return (TRUE);
	}
	return (FALSE);
}

int	check_burnout_all_coders(t_manager *manager)
{
	int			i;
	int			output;
	long long	time_of_death;

	i = -1;
	output = FALSE;
	while (++i < manager->utils_const[NB_CODERS])
	{
		pthread_mutex_lock(&manager->coders[i].coder_mutex);
		output = check_burnout(&manager->coders[i]);
		pthread_mutex_unlock(&manager->coders[i].coder_mutex);
		if (output == TRUE)
		{
			time_of_death = get_time() - manager->utils_const[TM_START];
			pthread_mutex_lock(&manager->mutex_print);
			printf("%lld %d has burnout\n",
				time_of_death, manager->coders[i].id);
			pthread_mutex_unlock(&manager->mutex_print);
			pthread_mutex_lock(&manager->mutex_manager);
			manager->end_type = BURNOUT_ERROR;
			pthread_mutex_unlock(&manager->mutex_manager);
			break ;
		}
	}
	return (output);
}

int	check_burnout(t_coder *coder)
{
	long long	norm;
	long long	time_check;
	long long	temp_last_compile;

	time_check = get_time();
	if (coder->id % 2 == 0)
		temp_last_compile = coder->last_compile - 1;
	else
		temp_last_compile = coder->last_compile;
	norm = time_check - coder->utils_const[TM_START];
	if (norm - temp_last_compile > coder->utils_const[TM_BURNOUT])
		return (TRUE);
	return (FALSE);
}
