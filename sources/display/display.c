/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfrancis <mfrancis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 14:35:32 by mfrancis          #+#    #+#             */
/*   Updated: 2024/12/16 14:35:32 by mfrancis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * @brief Handles the processing of AST (Abstract Syntax Tree) 
 * and heredoc management.
 * 
 * This function processes the AST by handling heredoc if necessary, and
 * executing the AST if no signal interruption occurs. If a signal is set, 
 * it frees the AST and sets it to `NULL`. It also sets up signal handling 
 * for execution and runs the AST through the `execute_ast` function.
 * 
 * @param minishell Pointer to the `t_minishell` structure
 * @param ast Pointer to the `t_ast`
 * 
 * @return Void. Performs operations on the minishell's state.
 */

static void	process_util(t_minishell *minishell, t_ast *ast)
{
	if (minishell->nbr_heredoc > 0)
	{
		process_ast_heredoc(minishell, minishell->ast, minishell->ast);
		minishell->nbr_heredoc = 0;
	}
	if (g_signal)
	{
		free_all(minishell, "");
		minishell->ast = NULL;
	}
	else
	{
		minishell->first = 0;
		setup_signals_executer();
		execute_ast(minishell, ast, -1);
		free_ast(minishell->ast);
		minishell->ast = NULL;
	}
}
/**
 * @brief Checks and marks tokens within the token list as part of 
 * a double-quoted string.
 * 
 * This function iterates over the tokens in the minishell's token list and 
 * marks tokens that are part of a double-quoted string by setting the `dq`
 * flag. It resets the flag when encountering a PIPE token. 
 * 
 * @param minishell Pointer to the `t_minishell` structure.
 * 
 * @return Void. This function does not return anything but modifies the flag.
 */

static void	check_is_str(t_minishell *minishell)
{
	t_token	*temp;
	int		flag;

	flag = 0;
	temp = minishell->tokens;
	while (temp)
	{
		if ((temp->str[0] == '"' || temp->str[0] == '\'') && flag == 0)
		{
			temp->dq = 1;
			flag = 1;
		}
		else
			temp->dq = 0;
		if (temp->dq == 0 && ft_strchr(temp->str, '$'))
			temp->expander = 1;
		else
			temp->expander = 0;
		if (temp->type == PIPE)
			flag = 0;
		temp = temp->next;
	}
}
/**
 * @brief Processes user input, performs tokenization, expands variables,
 * and executes commands.
 * 
 * This function handles the main flow for processing user input. It:
 * 1. Stops signals temporarily to ensure proper input handling.
 * 2. Tokenizes the user input and checks for syntax errors.
 * 3. Expands variables if the input is syntactically correct.
 * 4. Creates an AST from tokens and sets up redirections.
 * 5. Calls `process_util` to execute the AST or handles errors if the
 *  syntax is invalid.
 * 
 * @param minishell Pointer to the `t_minishell` structure.
 * 
 * @return Void. The function processes the input or returns without doing 
 * anything if the syntax is incorrect.
 */

static void	process_input(t_minishell *minishell)
{
	t_ast	*ast;

	stop_signals();
	tokenization(minishell);
	check_is_str(minishell);
	if (check_syntax(minishell->tokens) == true)
	{
		expander(minishell);
		minishell->ast = create_ast(minishell->tokens);
		ast = minishell->ast;
		if (!minishell->ast)
			return ;
		minishell->tokens = NULL;
		set_redirs(minishell, minishell->ast);
		process_util(minishell, ast);
	}
	else
	{
		g_signal = 2;
		return ;
	}
}
/**
 * @brief Displays the shell prompt, handles user input, and processes commands.
 * If the input is invalid or empty, it frees allocated resources
 * and exits as necessary.
 * Also updates the shell's exit status based on signals.
 * 
 * @param minishell Pointer to the `t_minishell` structure containing data.
 * @return Void. The function continues the shell loop or exits the program.
 */

void	display_prompt(t_minishell *minishell)
{
	if (!minishell->display)
		minishell->display = readline("[minishell]$ ");
	if (g_signal != 0)
	{
		minishell->exit_status = g_signal;
		g_signal = 0;
	}
	if (!minishell->display)
		free_exit(minishell, "");
	if (!*minishell->display)
	{
		free_all(minishell, "");
		return ;
	}
	add_history(minishell->display);
	process_input(minishell);
	free_all(minishell, "");
}
