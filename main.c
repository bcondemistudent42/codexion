/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcondemi <bcondemi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/23 17:42:19 by bcondemi          #+#    #+#             */
/*   Updated: 2026/05/23 19:53:18 by bcondemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "header.h"

int main(int ac, char **argv)
{
// to see eror when an letter in int 
// to see problem with zero
	printf("%d\n", parser_manager(ac, argv));
	if (parser_manager(ac, argv) == -1)
	{
		return (ft_error());
	}
}