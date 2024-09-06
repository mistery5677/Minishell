/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfrancis <mfrancis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 21:42:32 by mfrancis          #+#    #+#             */
/*   Updated: 2024/09/06 16:10:51 by mfrancis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int main(int argc, char *argv[], char *env[])
{
    (void)argc;
    (void)argv;
	init_minishell(env);
	while (1)
	{
      
        
    }
    printf("teste\n");
    return (0);
}

  /*    
        [] Inicialization of data/ minishell;
        [] infinit loop
            [] tratamento dos sinais - interactive mode;
            [] display_prompt - minishell$
            [] ler input  - GNL ou realine
                [] token
                [] Parsing
                [] execution
                    [] build-in or system ?
            [] free(input);
            o miguel e um toto.
        */