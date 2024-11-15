/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaabdall <yaabdall@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 01:46:03 by yaabdall          #+#    #+#             */
/*   Updated: 2024/11/11 21:37:31 by yaabdall         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_node	*create_ast_node(t_node_type type, char *value, t_gc *gc)
{
	t_node	*node;

	node = gc_malloc(sizeof(t_node), gc);
	if (!node)
		return (NULL);
	node->type = type;
	node->value = value;
	node->left = NULL;
	node->right = NULL;
	node->next = NULL;
	return (node);
}
