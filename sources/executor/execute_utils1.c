/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mistery576 <mistery576@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 23:34:18 by mistery576        #+#    #+#             */
/*   Updated: 2024/12/18 11:59:51 by mistery576       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Count the total number of commands, in the node
 * @param t_minishell *minishell, t_ast *ast
 * @return int
 */
int	count_total_strings(t_minishell *minishell, t_ast *ast)
{
	t_ast	*temp;
	int		total;
	int		i;

	total = 0;
	temp = ast;
	while (temp->token->type <= 3)
	{
		if (temp->token->cmd)
		{
			i = 0;
			while (temp->token->cmd[i])
			{
				total++;
				i++;
			}
		}
		if (open_file(minishell, temp) == -1)
		{
			failed_to_open(minishell);
			return (-1);
		}
		temp = temp->right;
	}
	return (total);
}

// Function to split a string by spaces and concatenate to cmd
char	**split_cmd(char **cmd, const char *str)
{
	char	**split_str;

	split_str = ft_split(str, ' ');
	return (concat_arrays(cmd, split_str));
}

void	process_token(t_ast *ast, int *count, char ***cmd)
{
	if (ft_strlen(ast->token->str) > 0)
	{
		*count += ft_count_words(ast->token->str);
		*cmd = split_cmd(*cmd, ast->token->str);
	}
}

/**
 * @brief Collects the commads from the AST
 * @param t_ast *ast, int *count, char **cmd
 * @return char**
 */
char	**collect_commands(t_minishell *minishell, t_ast *ast)
{
	char	**cmd ;
	int		count;
	int		first;

	first = 0;
	cmd = NULL;
	count = 0;
	while (ast)
	{
		if (first == 0 && ast->token->dq == 1
			&& ft_count_words(ast->token->str) > 1)
			minishell->_str_ = 1;
		if (ast->token->expander == 1 && ast->token->dq == 0)
			process_token(ast, &count, &cmd);
		else
			cmd = process_ast_commands(ast, &count, cmd);
		ast = ast->right;
		first = 1;
	}
	if (!cmd)
		return (NULL);
	return (cmd);
}

/**
 * @brief Collects the commands and execute them
 * @param t_minishell *minishell, t_ast *ast, int flag
 * @return void
 */
void	execute_redir(t_minishell *minishell, t_ast *ast, int flag)
{
	char	**cmd;

	cmd = collect_commands_redirs(minishell, ast);
	if (cmd == NULL)
	{
		rebuild_fileno(minishell);
		close_redir(minishell);
		if (minishell->_pipe_ == 1)
			free_exit(minishell, "");
		free_all(minishell, "");
		return ;
	}
	if (flag == -1)
		ft_execute(minishell, cmd);
	else if (minishell->_pipe_ == 1)
		ft_execute_pipe(minishell, cmd);
	rebuild_fileno(minishell);
	close_redir(minishell);
	return ;
}
