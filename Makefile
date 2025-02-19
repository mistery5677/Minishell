MAKEFLAGS += --no-print-directory

GREEN   		= \033[0;32m
TEAL    		= \033[1;36m
RESET 			= \033[0m 

NAME			=	minishell

CC				=	cc

CFLAGS			=	-Wall -Wextra -Werror -gdwarf-4 #-fsanitize=thread

RM				=	rm -rf

INC_DIR			=	includes/

SRC_DIR			=	sources/

OBJ_DIR			=	objs/

#PROG_DIR		=	program/

PARSER_DIR		=	parser/

SIGNALS_DIR		=	signals/

BUILT_INS_DIR	= 	built_ins/

EXECUTOR_DIR	=	executor/

#MATH_DIR		=	math/

#TYPES_DIR		=	t_types/

RANDOM_MALLOC	=	-Xlinker --wrap=malloc

ERROR_DIR		=	errors/

DISPLAY_DIR		=	display/

LIBS_DIR		=	libraries/

LFT_PATH		=	$(addprefix $(LIBS_DIR), libft/)

SRC				=	$(addprefix $(SRC_DIR), minishell.c) \
					$(addprefix $(SRC_DIR)$(SIGNALS_DIR), signal.c signal2.c) \
					$(addprefix $(SRC_DIR)$(DISPLAY_DIR), display.c find_files.c find_files_utils.c inits.c) \
					$(addprefix $(SRC_DIR)$(ERROR_DIR), errors.c errors1.c errors2.c errors_execute.c) \
					$(addprefix $(SRC_DIR)$(BUILT_INS_DIR), pwd.c exit.c) \
					$(addprefix $(SRC_DIR)$(BUILT_INS_DIR), env.c) \
					$(addprefix $(SRC_DIR)$(BUILT_INS_DIR), export.c local_export.c) \
					$(addprefix $(SRC_DIR)$(BUILT_INS_DIR), built_ins_utils.c) \
					$(addprefix $(SRC_DIR)$(BUILT_INS_DIR), unset.c) \
					$(addprefix $(SRC_DIR)$(BUILT_INS_DIR), echo.c) \
					$(addprefix $(SRC_DIR)$(BUILT_INS_DIR), cd.c) \
					$(addprefix $(SRC_DIR)$(BUILT_INS_DIR), cd_utils.c) \
					$(addprefix $(SRC_DIR)$(EXECUTOR_DIR), execute.c heredoc.c heredoc_utils.c split_cmd.c) \
					$(addprefix $(SRC_DIR)$(EXECUTOR_DIR), pipes.c redir.c execute_utils.c execute_utils1.c execute_utils2.c) \
					$(addprefix $(SRC_DIR)$(EXECUTOR_DIR), my_getenv.c array_utils.c array_utils2.c) \
					$(addprefix $(SRC_DIR)$(PARSER_DIR), expander_vars.c replace_var_in_str.c) \
					$(addprefix $(SRC_DIR)$(PARSER_DIR), tokenization.c tokenization_utils.c expander.c syntax_error.c ast.c ast1.c parser_utils.c remove_quotes.c) \
#					$(addprefix $(SRC_DIR), malloc.c)

OBJ_DIRS		=	$(OBJ_DIR) $(addprefix $(OBJ_DIR), $(PARSER_DIR)) \
					$(addprefix $(OBJ_DIR), $(SIGNALS_DIR)) \
					$(addprefix $(OBJ_DIR), $(ERROR_DIR)) \
					$(addprefix $(OBJ_DIR), $(DISPLAY_DIR)) \
					$(addprefix $(OBJ_DIR), $(BUILT_INS_DIR)) \
					$(addprefix $(OBJ_DIR), $(EXECUTOR_DIR))

OBJ				=	$(SRC:$(SRC_DIR)%.c=$(OBJ_DIR)%.o)

$(NAME):			$(OBJ) | $(SRC) 
# Pc da escola					$(CC) $(CFLAGS) -lreadline $^ -o $@ -I $(INC_DIR) -L$(LFT_PATH) -lft
					$(CC) $(CFLAGS) $^ -o $@ -I $(INC_DIR) -L$(LFT_PATH) -lft -lreadline -lncurses
					printf '$(GREEN)$(NAME) compiled successfully!\n $(RESET)'

$(OBJ_DIR)%.o:		$(SRC_DIR)%.c | $(OBJ_DIRS)
					make dependencies
					printf "$(TEAL)Compiling $(NAME) objects... %-33.33s\n$(RESET)" $(notdir $@) 
					$(CC) $(CFLAGS) -c $< -o $@ -I $(INC_DIR) 

$(OBJ_DIRS):
					mkdir -p $@

all:				$(NAME)

random_malloc:		$(OBJ) | $(SRC)
					$(CC) $(CFLAGS) $(RANDOM_MALLOC) -lreadline $^ -o $(NAME) -I $(INC_DIR) -L$(LFT_PATH) -lft
					printf '$(GREEN)$(NAME) compiled successfully!\n $(RESET)'

dependencies:
					make -C $(LFT_PATH)

clean:
					if [ -d $(OBJ_DIR) ]; then $(RM) $(OBJ_DIR); fi
					make clean -C $(LFT_PATH)

fclean:				clean
					$(RM) $(NAME)
					make fclean -C $(LFT_PATH)

re:					fclean all

val: $(NAME)
	make re 
	clear 
	valgrind --suppressions=readline.supp --leak-check=full --show-leak-kinds=all --track-fds=all --track-origins=yes ./$(NAME)

.PHONY:				all clean fclean re val

