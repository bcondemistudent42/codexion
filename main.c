/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcondemi <bcondemi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/23 17:42:19 by bcondemi          #+#    #+#             */
/*   Updated: 2026/05/26 13:13:05 by bcondemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

int	main(int ac, char **argv)
{
	printf("%d\n", parser_manager(ac, argv));
	if (parser_manager(ac, argv) == -1)
	{
		return (ft_error());
	}
}
