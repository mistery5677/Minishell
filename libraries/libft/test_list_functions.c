/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_list_functions.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfrancis <mfrancis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 08:56:05 by mfrancis          #+#    #+#             */
/*   Updated: 2024/06/23 18:05:17 by mfrancis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// void	print_data_a_front(t_data *data)
// {
// 	t_node	*current;
//     ft_printf("LISTA A HEAD:");
// 	current = data->a_head;
//     if (current == NULL)
//     {
//         ft_printf("A lista A está vazia.\n");
//         return;
//     }
// 	if (current->prev == NULL)
// 		ft_printf("NULL0 ");
// 	while (current)
// 	{
// 		ft_printf("%d ", current->value);
// 		current = current->next;
// 	}
// 	if (data->a_tail && data->a_tail->next == NULL)
//         ft_printf("NULL1\n");
// }
// void	print_data_b_front(t_data *data)
// {
// 	t_node	*current;
//     ft_printf("LISTA B HEAD:");
// 	current = data->b_head;
//     if (current == NULL)
//     {
//         ft_printf("A lista B está vazia.\n");
//         return;
//     }
// 	if (current->prev == NULL)
// 		ft_printf("NULL0 ");
// 	while (current)
// 	{
// 		ft_printf("%d ", current->value);
// 		current = current->next;
// 	}
// 	if (data->b_tail && data->b_tail->next == NULL)
//         ft_printf("NULL1\n");
// }
// void	print_data_a_back(t_data *data)
// {
// 	t_node	*current;
//     ft_printf("LISTA A TAIL:");
// 	current = data->a_tail;
//      if (current == NULL)
//     {
//         ft_printf("A lista A está vazia.\n");
//         return;
//     }
// 	if (current->next == NULL)
// 		ft_printf("NULL0 ");
// 	while (current != NULL)
// 	{
// 		ft_printf("%d ", current->value);
// 		current = current->prev;
// 	}
// 	  if (data->a_head && data->a_head->prev == NULL)
//         ft_printf("NULL1\n");
// }
// void	print_data_b_back(t_data *data)
// {
// 	t_node	*current;
//     ft_printf("LISTA B TAIL:");
// 	current = data->b_tail;
//      if (current == NULL)
//     {
//         ft_printf("A lista B está vazia.\n");
//         return;
//     }
// 	if (current->next == NULL)
// 		ft_printf("NULL0 ");
// 	while (current != NULL)
// 	{
// 		ft_printf("%d ", current->value);
// 		current = current->prev;
// 	}
// 	  if (data->b_head && data->b_head->prev == NULL)
//         ft_printf("NULL1\n");
// }
