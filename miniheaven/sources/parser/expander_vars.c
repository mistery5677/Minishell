/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_vars.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mistery576 <mistery576@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 21:12:49 by mistery576        #+#    #+#             */
/*   Updated: 2025/01/16 16:27:37 by mistery576       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	expand_vars_utils1(t_minishell *minishell, t_token *token, int *idx)
{
	if (token->str[*idx] == '\'')
	{
		if (!minishell->expander->dq)
			rm_single_quotes(token, minishell, idx);
	}
	else if (token->str[*idx] == '"')
	{
		rm_double_quote(token, minishell, idx);
		minishell->expander->dq = !minishell->expander->dq;
	}
}

static void	support(t_minishell *minishell,
	t_token *token, int *idx, char **new_str)
{
	if (minishell->expander->dq || !minishell->expander->dq)
	{
		*idx = *idx - ft_strlen(minishell->expander->var_name) - 1;
		*new_str = replace_var_in_str(token->str, minishell, idx);
		free(token->str);
		token->str = *new_str;
		if (minishell->expander->dq
			&& ft_count_words(minishell->expander->var_content) > 1)
			token->type = STR;
	}
}

static void	expand_vars_utils2(t_minishell *minishell,
	t_token *token, int *idx, char **new_str)
{
	(*idx)++;
	if (token->str[*idx] == '?')
	{
		minishell->expander->var_name = ft_strdup("?");
		(*idx)++;
	}
	else if (!ft_isalpha(token->str[*idx]) && token->str[*idx] != '_' )
	{
		minishell->expander->var_name
			= add_char(token->str[*idx], minishell->expander->var_name);
		(*idx)++;
	}
	else
	{
		while (token->str[*idx]
			&& (ft_isalnum(token->str[*idx])
				|| token->str[*idx] == '_'))
		{
			minishell->expander->var_name
				= add_char(token->str[*idx], minishell->expander->var_name);
			(*idx)++;
		}
	}
	minishell->expander->var_content = extrat_var_content(minishell);
	support(minishell, token, idx, new_str);
}

void	handle_expansion_cases(t_minishell *minishell,
		t_token *token, int *idx, char **new_str)
{
	if (token->str[*idx] == '$' && token->str[*idx + 1] == '\''
		&& !minishell->expander->dq)
	{
		rm_dollar(token, minishell, idx);
		rm_single_quotes(token, minishell, idx);
	}
	else if (token->str[*idx] == '$' && token->str[*idx + 1] == '"'
		&& !minishell->expander->dq)
	{
		rm_dollar(token, minishell, idx);
		(*idx)--;
	}
	else if (token->str[*idx] == '\'' || token->str[*idx] == '"')
	{
		expand_vars_utils1(minishell, token, idx);
	}
	else if (token->str[*idx] == '$' && !token->str[*idx + 1]
		&& !minishell->expander->dq)
		return ;
	else if (token->str[*idx] == '$')
	{
		expand_vars_utils2(minishell, token, idx, new_str);
		free_expand(minishell->expander);
	}
}

void	expand_vars(t_minishell *minishell, t_token *token)
{
	int		idx;
	char	*new_str;

	idx = 0;
	if (minishell->expander == NULL)
		minishell->expander = malloc(sizeof(t_expand));
	if (!minishell->expander)
		free_exit(minishell, "Error - Fail allocating expander");
	reset_expand(minishell->expander);
	while (token->str[idx])
	{
		handle_expansion_cases(minishell, token, &idx, &new_str);
		idx++;
	}
}
