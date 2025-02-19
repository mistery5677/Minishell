/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfrancis <mfrancis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 14:33:05 by mfrancis          #+#    #+#             */
/*   Updated: 2024/12/16 14:33:05 by mfrancis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	g_signal;

/**
 * @brief Entry point for the minishell program.
 * 
 * Initializes the shell environment, sets up signal handling, 
 * and enters a loop to display the shell prompt and process commands.
 * 
 * @param argc The argument count. Must be 1 for the program to proceed.
 * @param argv The argument vector. Unused in this implementation.
 * @param envp The environment variables passed to the shell.
 * 
 * @return 0 upon successful execution. or exits the program.
 */
int	main(int argc, char *argv[], char *envp[])
{
	t_minishell	minishell;

	(void)argv;
	if (argc != 1)
		exit(0);
	initialization(envp, &minishell);
	while (1)
	{
		setup_signals();
		display_prompt(&minishell);
	}
	free_exit(&minishell, "");
	return (0);
}

/*
			[/] Inicialization of data/ minishell;
				[/] structs
				[x] envp
				[x] SHLVL
			[/] setup_signals - interective signals
				[x]  crtl D
				[x]  crtl \
				[x]  crtl C
			[?] display_prompt - rl_redisplay minishell$
				[x] ver leaks - suprimir - supression file - .supp
			[] ler input  - readline ?
				[] Lexing
					[] token
						[x] funcao gettoken
							[x] incrementaro o idx ate encontrar um dos simbolos
								[x] se encontra > ou < ver se tem outro a seguir
									e dar outro tipo de char ?
							[x] split split da palavra se tiver antes do simbolo
							[x] split do simbolo
							[x] trim p limpar os withespaces;
				[] Parser
					[] BNF (apenas um doc para mim p conseguir fazer o sintax 
					error)
					'--> [] Error sintax
							'--> [] AST
				[/] execution
					[/] build-in or system ?
						[x]  pwd (Finito,
							e sem memory leaks e sem erros de norma)
						[x]  cd (Finito,
							e sem memory leaks e sem erros de norma)
							[x] Corrigir o bug,
								quando estou no dir '/' e ando para outro 
								diretorio,
								o output é '//home'
						[x]  env (Finito,
							e sem memory leaks e sem erros de norma)
						[x]  export (Finito,
							e sem memory leaks e sem erros de norma)
							[x] Não pode aceitar variaveis com numeros
							[x] Dar export de local variables

						[/]  echo -n (Sem memory leaks,
							norma falta o header. Falta considerar algumas 
							condicoes)
						[x]  unset (Finito,
							e sem memory leaks e sem erros de norma)
						[x]  exit (Finito,
							fiz o reaproveitamento da funcao que ja existe)
			[x] free(input);
			[x] Executor;
				[x] Documentacao (Finito)
				[x] Aceitar os comandos a partir do diretorio, por exemplo
					/bin/ls
			[x] CORRIGIR PIPEX;
				[x] Caso nao exista o infile,
					mas ja existe informacao no outfile,
					tenho de limpar quando ex (infile ls wc outfile)
				[] Adicionar a funcionalidade "here_doc"
				[] Caso o numero de argumentos esteja errado no here_doc,
					a mensagem de erro aparece dentro do outfile
			[] LOCAL VARIABLES
				[x] Adicionar uma variavel local, à minha lista
				[x] Verificar se já existe a variavel local
					[x] Caso exista, tenho de alterar o content da varável
				[x] Comportar-se com export
				[x] Comportar-se com unset
				[x] Verificar se a variavel ja existe no envp
				[] Dar erro quando teste=" ou teste='
				[] Caso faça teste=! e dps eu fizer export teste=1,
					tenho de tirar da local_var e exportar com o novo valor
				OUTPUTS ESTRANHOS
					[] test= | A variavel fica com o valor nulo
					[x] unset teste | A função unset tb funciona para variaveis 
					locais
			[x] Alterar o Shell level, quando executamos outra vez o minishell
			[x] Aceitar comandos como por exemplo "ls -l"

			PENDÊNCIAS
			[] Variaveis locais com aspas

			MIGUEL CORRIGIR HOJE:
			[] export
				[] a="ls -l -a"
*/
