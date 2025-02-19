/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfrancis <mfrancis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 16:42:05 by mfrancis          #+#    #+#             */
/*   Updated: 2024/12/16 16:42:05 by mfrancis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libraries/libft/libft.h"
# include <dirent.h>
# include <errno.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

extern int	g_signal;

typedef enum s_token_type
{
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND,
	REDIR_HEREDOC,
	WORD,
	STR,
	PIPE,
	NO_TYPE,
}	t_tk_tp;

typedef struct s_heredoc
{
	struct s_heredoc	*next;
	char				*delimiter;
	char				*line;
	int					fd[2];
	int					quotes;
	int					id;
}	t_heredoc;

typedef struct s_expand
{
	char	*var_name;
	char	*var_content;
	int		dq;
	int		sq;
}	t_expand;

typedef struct s_token
{
	struct s_token	*next;
	t_tk_tp			type;
	char			*str;
	char			*path;
	char			**cmd;
	int				expander;
	int				dq;
}	t_token;

typedef struct s_ast
{
	struct s_ast	*left;
	struct s_ast	*right;
	t_token			*token;
}	t_ast;

typedef struct s_minishell
{
	t_token		*tokens;
	t_ast		*ast;
	t_expand	*expander;
	t_heredoc	*heredoc;
	char		**envp;
	char		**local;
	char		*display;
	char		*cmd;
	int			_str_;
	int			first;
	int			fd[2];
	int			exit_status;
	int			prev_fd;
	int			temp_stdin;
	int			temp_stdout;
	int			infile;
	int			outfile;
	int			commands;
	int			_pipe_;
	int			nbr_heredoc;
}	t_minishell;

// Inits
void		initialization(char *envp[], t_minishell *minishell);

// Display Prompt
void		display_prompt(t_minishell *minishell);

// signals
void		setup_signals(void);
void		stop_signals(void);
void		setup_signals_here(void);
void		setup_signals_executer(void);

// tokanization
void		tokenization(t_minishell *minishell);
int			check_quotes(char quote, t_minishell *minishell, int *idx);
int			process_quotes(t_minishell *minishell, int *idx, char symb);

// tokenization_utils
void		process_op_tokens(t_minishell *minishell, int *start, int *idx);
void		add_operator_word(t_minishell *minishell,	
				int *start, int idx, int token_type);	
void		add_word(t_minishell *minishell, int *start, int *idx);	
void		add_simple_token(t_minishell *minishell, int *start, int *idx);	
void		add_token_back(t_minishell *minishell, char *token, t_tk_tp type);

// syntax_error
bool		check_syntax(t_token *tokens);
bool		check_syntax_pipe(t_token *temp, t_token *last_token);
bool		check_syntax_redir(t_token *temp);

// expander
void		expander(t_minishell *minishell);
void		reset_expand(t_expand *expander);
char		*add_char(int c, char *str);
char		*extrat_var_content(t_minishell *minishell);

// expander vars
void		expand_vars(t_minishell *minishell, t_token *token);
void		rm_dollar(t_token *token, t_minishell *minishell, int *idx);

// replace_var_in_str
char		*replace_var_in_str(char *str, t_minishell *minishell, int *idx);

// parser_utils
int			ft_count_words(char const *s);
int			is_delimiter(char c);

// remove_quotes
void		rm_one_single_quotes(t_token *token,
				t_minishell *minishell, int *idx);
void		rm_single_quotes(t_token *token, t_minishell *minishell, int *idx);
void		rm_double_quote(t_token *token, t_minishell *minishell, int *idx);

// ast
t_ast		*create_ast(t_token *tokens);
t_tk_tp		check_redir(t_token *tokens);
int			check_symbol(t_token *tokens, t_tk_tp symbol);
t_ast		*add_right(t_token **tokens);
t_ast		*new_ast_node(t_token *tokens);

// ast1
t_ast		*split_tokens_ast(t_token **tokens, t_tk_tp symbol);

//	find_files
t_ast		*copy_ast(t_ast *original);
void		no_pipe(t_ast *temp_copy, t_ast *orig, t_ast *save_node);
void		find_files(t_ast *orig, t_ast *temp_copy, t_minishell *minishell);

// find_files_utils
char		**built_cmd(t_ast *ast);
void		set_redirs(t_minishell *minishell, t_ast *ast);
void		no_pipe_util(t_ast *orig, t_ast *temp_copy);
void		complete_last_redir(t_ast *temp_copy, t_ast *orig,
				t_ast *save_node);

// heredoc
void		process_ast_heredoc(t_minishell *minishell, t_ast *ast,
				t_ast *head);
void		process_heredoc(t_minishell *minishell, t_ast *ast, t_ast *top);
void		process_prompt(t_minishell *minishell, t_heredoc *heredoc,
				t_ast *top);
void		write_to_pipe(t_minishell *minishell, t_heredoc *heredoc);

void		heredoc_quotes(t_minishell *minishell, t_heredoc *heredoc);

//heredoc_utils
t_heredoc	*add_heredoc_node(t_minishell *minishell);
char		*expand_heredoc(t_minishell *minishell, char *line);
void		get_var_name(t_expand *expander, int *i, char *line);
void		rm_here_quotes(t_minishell *minishell, t_heredoc *heredoc, int *idx,
				char quote);

// Built_ins //
// CD_UTILS //
char		*rebuild_pwd(char **split_pwd);
int			change_old_path(t_minishell *minishell, char *new_path);
int			change_pwd(t_minishell *minishell, char *new_path);

// CD //
void		ft_cd(char **split_cmd, t_minishell *minishell);

// ECHO //
void		ft_echo(char **cmd);
int			find_char(char *str, char c);

// ENV //
void		ft_env(t_minishell *minishell, char **cmd);

// EXPORT//
void		ft_export(t_minishell *minishell, char **cmd);
int			find_equal(char *var);
int			add_local(t_minishell *minishell, char *var);
int			bigger_var_name(char *original, char *step_ahead);
void		clear_local(t_minishell *minishell, char *var, int x);
int			check_local_env(t_minishell *minishell, char *var);

// PWD //
void		ft_pwd(void);

// UNSET //
void		ft_unset(char **cmd, t_minishell *minishell);

// EXIT //
void		ft_exit(t_minishell *minishell, char **dp);

// Built_ins_utils //
char		**dup_envp(t_minishell *minishell, char **envp); /*Usado no export*/
char		*find_path(t_minishell *minishell, char *cmd); /*Usado no env*/

// EXECUTOR //
void		execute_ast(t_minishell *minishell, t_ast *ast, int flag);
char		*my_getenv(t_minishell *minishell, char *path);
int			find_builtin(t_minishell *minishell, char **dp);
void		error_execute(t_minishell *minishell, char **split_cmd,
				char *cmd_path);
void		ft_execute(t_minishell *minishell, char **split_cmd);
void		execute_cmd(t_minishell *minishell, char **split_cmd);
char		**built_cmd(t_ast *ast);

//	EXECUTOR_UTILS  //
char		**collect_commands_redirs(t_minishell *minishell, t_ast *ast);
char		**collect_commands(t_minishell *minishell, t_ast *ast);
int			count_total_strings(t_minishell *minishell, t_ast *ast);
int			count_array(char **arr);
void		execute_redir(t_minishell *minishell, t_ast *ast, int flag);
char		**process_ast_commands(t_ast *ast, int *count, char **cmd);

// PIPEX //
void		do_pipeline(t_minishell *minishell, t_ast *ast);
void		pipe_fork(t_minishell *minishell, t_ast *ast);
pid_t		fork_and_pipe(t_minishell *minishell, t_ast *ast);
void		redir_pipe(t_minishell *minishell, int child);
void		ft_execute_pipe(t_minishell *minishell, char **cmd);

//	REDIR  //
int			open_file(t_minishell *minishell, t_ast *ast);
int			redirect_read(t_minishell *minishell);
void		close_redir(t_minishell *minishell);
int			rebuild_fileno(t_minishell *minishell);

//	SPLIT_CMD	//
char		**ft_split_cmd(char **cmd, int cmd_count);

//	ARRAY_UTILS	//
char		**join_array(char **array_first, char *array);

void		free_cmd_path(t_ast *left_side);
void		find_files(t_ast *orig, t_ast *temp_copy, t_minishell *minishell);

// errors
void		free_exit(t_minishell *minishell, char *str);
void		free_all(t_minishell *minishell, char *str);
void		fd_clean(void);
void		free_cmd_path(t_ast *left_side);
char		**concat_arrays(char **arr1, char **arr2);
char		**replace_null_with_empty(char **str);
int			copy_token_to_array(t_ast *ast, char **cmd_array, int *i);
char		**join_arrays(char **array_first, char *array);

//errors1
void		free_array(char **array);
void		free_tokens(t_minishell *minishell);
void		free_expand(t_expand *expander);
void		free_ast(t_ast *ast);
void		free_heredoc(t_heredoc *heredoc);

//errors2
void		close_pipe(int fd);
void		free_pointer(void *ptr);

//errors execute
void		print_errors(char *s1, char *s2, char *s3);
int			check_execute(t_minishell *minishell, char **cmd);
char		*failed_to_open(t_minishell *minishell);

// split_token
// void		split_tokens(t_minishell *minishell, t_token **current);
// void		add_token_between(t_minishell *minishell, t_token *tokens,
// 				char *str);

// int			find_last_delimiter(char *str);
// debug functions
// void		print_token(t_minishell *data);
// void		print_token2(t_token *data);
// char	*test (char *str);
// void		print_ast(t_ast *ast, int level);
// char		*token_type_to_str(t_tk_tp type);
// void		print_heredoc(t_minishell *data);
#endif