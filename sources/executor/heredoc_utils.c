/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfrancis <mfrancis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 16:18:32 by mfrancis          #+#    #+#             */
/*   Updated: 2024/12/16 16:18:32 by mfrancis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Adds a new heredoc node to the linked list in the minishell structure.
 * Allocates memory for a new `t_heredoc` node and initializes its fields. 
 * 
 * @param minishell Pointer to the `t_minishell` structure.
 * 
 * @return Pointer to the newly created `t_heredoc` node.
 */
t_heredoc	*add_heredoc_node(t_minishell *minishell)
{
	t_heredoc	*new_node;
	t_heredoc	*temp;

	new_node = malloc(sizeof(t_heredoc));
	if (!new_node)
		free_exit(minishell, "Error - Fail allocating heredoc\n");
	new_node->delimiter = NULL;
	new_node->fd[0] = -1;
	new_node->fd[1] = -1;
	new_node->quotes = 0;
	new_node->id = -1;
	new_node->next = NULL;
	if (!minishell->heredoc)
		minishell->heredoc = new_node;
	else
	{
		temp = minishell->heredoc;
		while (temp->next != NULL)
			temp = temp->next;
		temp->next = new_node;
	}
	return (new_node);
}

/**
 * @brief Handles variable replacement in a heredoc line.
 * 
 * @param line The current line being processed.
 * @param i Pointer to the current index of the line.
 * @param minishell Pointer to the `t_minishell` structure.
 * 
 * @return The newly constructed string with the variable replaced.
 */
char	*process_variable_replacement(char *line, int *i,
			t_minishell *minishell)
{
	char	*new_str;
	int		offset;

	offset = 0;
	new_str = NULL;
	(*i)++;
	get_var_name(minishell->expander, i, line);
	minishell->expander->var_content = extrat_var_content(minishell);
	offset = *i - ft_strlen(minishell->expander->var_name) - 1;
	new_str = replace_var_in_str(line, minishell, &offset);
	free(line);
	free_expand(minishell->expander);
	return (new_str);
}

/**
 * @brief Expands environment variables in a heredoc line.
 * 
 * Iterates through a heredoc line and expands any variables by 
 * replacing them with their corresponding values.
 * 
 * @param minishell Pointer to the `t_minishell` structure.
 * @param line The line to expand.
 * 
 * @return The expanded line.
 */
char	*expand_heredoc(t_minishell *minishell, char *line)
{
	int		i;
	char	*new_str;

	i = 0;
	new_str = line;
	if (minishell->expander == NULL)
	{
		minishell->expander = malloc(sizeof(t_expand));
		if (!minishell->expander)
			free_exit(minishell, "Error - Fail allocating expander");
	}
	reset_expand(minishell->expander);
	while (line[i])
	{
		if (line[i] == '$')
		{
			new_str = process_variable_replacement
				(line, &i, minishell);
			line = new_str;
		}
		i++;
	}
	return (new_str);
}

/**
 * @brief Extracts the variable name from a line for variable expansion.
 * 
 * @param expander Pointer to the `t_expand` structure.
 * @param i Pointer to the current index in the line.
 * @param line The line being processed.
 * 
 * @return Void. Updates the `var_name` field in the expander.
 */
void	get_var_name(t_expand *expander, int *i, char *line)
{
	if (line[*i] == '?')
	{
		expander->var_name = ft_strdup("?");
		(*i)++;
	}
	else if (!ft_isalpha(line[*i]) && line[*i] != '_')
	{
		expander->var_name = add_char(line[*i], expander->var_name);
		(*i)++;
	}
	else
	{
		while (line[*i] && (ft_isalnum(line[*i]) || line[*i] == '_'))
		{
			expander->var_name = add_char(line[*i], expander->var_name);
			(*i)++;
		}
	}
}

/**
 * @brief Removes quotes from a heredoc delimiter.
 * 
 * Processes a heredoc delimiter to remove single or double  quotes.
 * Allocates a new string without the quotes and updates the delimiter.
 * 
 * @param minishell Pointer to the `t_minishell` struct.
 * @param heredoc Pointer to the `t_heredoc` struct.
 * @param idx Pointer to the current index in the delimiter.
 * @param quote The type of quote to remove (`'` or `"`).
 * 
 * @return Void. Updates the `delimiter` field in the heredoc struct.
 */
void	rm_here_quotes(t_minishell *minishell, t_heredoc *heredoc, int *idx,
		char quote)
{
	int		start;
	int		end;
	char	*new_str;
	int		new_idx;

	start = *idx + 1;
	end = start;
	new_idx = 0;
	while (heredoc->delimiter[end] != quote)
		end++;
	new_str = malloc(sizeof(char) * (ft_strlen(heredoc->delimiter) - 1));
	if (!new_str)
		free_exit(minishell, "Error - Fail allocating memory for new str\n");
	ft_memcpy(new_str, heredoc->delimiter, start - 1);
	new_idx = start - 1;
	ft_memcpy(new_str + new_idx, heredoc->delimiter + start, end - start);
	new_idx = new_idx + (end - start);
	ft_memcpy(new_str + new_idx, heredoc->delimiter + end + 1,
		(ft_strlen(heredoc->delimiter + end + 1) + 1));
	free(heredoc->delimiter);
	heredoc->delimiter = new_str;
	*(idx) = new_idx - 1;
}
