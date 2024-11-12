CC = cc
CFLAGS = -Wall -Wextra -Werror -fsanitize=thread -g

NAME = philo
SRC = main.c \
      parsign.c \
	  tools.c \
	  routine.c

OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $@

clean:
	@rm -rf $(OBJ)

fclean: clean
	@rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re
