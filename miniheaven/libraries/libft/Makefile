.SILENT:
MAKEFLAGS += --no-print-directory

CC = cc
CFLAGS = -Wall -Wextra -Werror
NAME = libft.a
LIBC = ar rcs
RM = rm -f
SRCS =	ft_isalpha.c \
		ft_isdigit.c \
		ft_isalnum.c \
		ft_isascii.c \
		ft_isprint.c \
		ft_strlen.c \
		ft_memset.c \
		ft_bzero.c \
		ft_memcpy.c \
		ft_memmove.c \
		ft_strlcpy.c \
		ft_strlcat.c \
		ft_toupper.c \
		ft_tolower.c \
		ft_strchr.c \
		ft_strrchr.c \
		ft_strncmp.c \
		ft_memchr.c \
		ft_memcmp.c \
		ft_strnstr.c\
		ft_atoi.c \
		ft_strdup.c \
		ft_calloc.c \
		ft_substr.c \
		ft_strjoin.c \
		ft_strtrim.c \
		ft_split.c \
		ft_itoa.c \
		ft_strmapi.c \
		ft_striteri.c \
		ft_putchar_fd.c \
		ft_putstr_fd.c \
		ft_putendl_fd.c \
		ft_putnbr_fd.c \
		ft_atol.c \
		ft_lstnew.c \
		ft_lstadd_front.c \
		ft_lstsize.c \
		ft_lstlast.c \
		ft_lstadd_back.c \
		ft_lstdelone.c \
		ft_lstclear.c \
		ft_lstiter.c \
		ft_lstmap.c \
		ft_printf.c \
		ft_putchar.c \
		ft_putstr.c \
		ft_puthexadecimal.c \
		ft_putnbr.c \
		ft_putpointer.c \
		ft_putunsigned.c \
		get_next_line_bonus.c \
		get_next_line_utils_bonus.c\
		test_list_functions.c \
		ft_strstr.c \
		ft_strcmp.c \
		ft_realloc.c

OBJECTS = $(SRCS:.c=.o)

BONUS_OBJECTS = $(BONUS:.c=.o)	

all: $(NAME)

.c: .o
	$(CC) $(CFLAGS) -c $< -o $(<:.c=.o)

$(NAME): $(OBJECTS) 
		$(LIBC) $(NAME) $(OBJECTS)

clean:
	$(RM) $(OBJECTS)

fclean: clean
		$(RM) $(NAME)

re: fclean all


.PHONY: all, clean, fclean, re