/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besch <besch@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 02:54:14 by yaabdall          #+#    #+#             */
/*   Updated: 2024/11/13 23:04:25 by besch            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// static void	substitute_command(char *input, int *i, int *count, t_minishell *data)
// {
// 	int		cmd_start;
// 	int		cmd_end;
// 	char	*command_str;

// 	(*i)++;
// 	cmd_start = *i;

// 	while (input[*i] && input[*i] != '`')
// 		(*i)++;
// 	if (input[*i] == '`')
// 	{
// 		cmd_end = *i;
// 		command_str = ft_substr_gc(input, cmd_start, cmd_end - cmd_start, &data->gc);
// 		data->tokens[*count] = create_token(COMMAND, command_str);
// 		*(count)++;
// 	}
// 	else
// 	{
// 		handle_unclosed_char(input, i, data);
// 	}
// }

static void	append_char_to_string(char *input, int *i, t_minishell *data)
{
	char	new_char;
	char	*temp_str;
	size_t	input_len;

	// Récupérer le caractère à ajouter (dans le contexte d'une citation, c'est l'un des caractères du contenu)
	new_char = input[*i];

	// Calculer la longueur actuelle de la chaîne d'entrée
	input_len = ft_strlen(input);

	// Allouer de la mémoire pour la nouvelle chaîne avec un caractère supplémentaire
	temp_str = gc_malloc(input_len + 2, &data->gc); // +1 pour le nouveau caractère, +1 pour '\0'

	// Copier le contenu actuel dans la nouvelle chaîne
	ft_strlcpy(temp_str, input, input_len + 1);

	// Ajouter le nouveau caractère à la fin de la chaîne
	temp_str[input_len] = new_char;
	temp_str[input_len + 1] = '\0'; // Assurer que la chaîne se termine bien

	// Mettre à jour la chaîne d'entrée avec la nouvelle chaîne
	ft_free(&input); // Libérer l'ancienne chaîne
	input = temp_str; // Réaffecter la chaîne d'entrée à la nouvelle chaîne
}

static int	find_matching_char(char *input, int char_index)
{
	char	match_char;

	match_char = input[char_index];
	char_index++;
	while (input[char_index])
	{
		if (input[char_index] == '\\' && input[char_index + 1] == match_char)
			char_index++;
		else if (input[char_index] == match_char)
			return (char_index);
		char_index++;
	}
	return (-1);
}

static int	handle_unclosed_char(char **input, int start, t_minishell *data)
{
	char	*additional_input;
	char	*temp;
	size_t	total_size;
	int		end;

	while (1)
	{
		write(1, "> ", 2);
		additional_input = get_next_line(0);
		if (!additional_input)
			return (-1);
		if (additional_input[ft_strlen(additional_input) - 1] == '\n')
			additional_input[ft_strlen(additional_input) - 1] = '\0';
		temp = *input;
		total_size = ft_strlen(temp) + ft_strlen(additional_input) + 1;
		*input = gc_malloc(total_size, &data->gc);
		ft_strlcpy(*input, temp, ft_strlen(temp) + 1);
		ft_strlcat(*input, additional_input, total_size);
		ft_free(&temp);
		ft_free(&additional_input);
		end = find_matching_char(*input, start);
		if (end != -1)
			return (end);
	}
	return (-1);
}

int	process_quotes(char *input, int *i, int *count, t_minishell *data)
{
	char			quote_char;
	int				start;
	int				end;

	quote_char = input[*i];
	start = *i;
	end = find_matching_char(input, start);
	(*i)++;
	if (end == -1)
	{
		end = handle_unclosed_char(&input, start, data);
		if (end == -1)
			return (-1);
	}
	while (*i < end)
	{
		if (quote_char == '"')
		{
			if (input[*i] == '$')
				process_env_var(input, i, count, data);
			else if (input[*i] == '\\' && (*i + 1 < end) && (input[*i + 1]
					== '$' || input[*i + 1] == '\\' || input[*i + 1] == '`'))
			{
				(*i)++;
				append_char_to_string(input, i, data);
			}
			// else if (input[*i] == '`')
			// 	substitute_command(input, i, count, data);
		}
		if (input[*i] == '\\' && input[*i + 1] == quote_char)
			(*i)++;
		(*i)++;
	}
	data->tokens[*count] = create_token(data->current_type, \
	ft_substr_gc(input, start + 1, end - start - 1, &data->gc));
	data->single_quote = (quote_char == '\'');
	data->double_quote = (quote_char == '"');
	(*i)++;
	(*count)++;
	return (0);
}
