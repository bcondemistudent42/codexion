NAME    = codexion

CC      = cc
CFLAGS  = -Wall -Werror -Wextra -g

SRC     = $(wildcard *.c)
OBJ     = $(addprefix obj/, $(SRC:.c=.o))

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

malloc_test: $(OBJ)
	$(CC) $(CFLAGS) -fsanitize=undefined -rdynamic $(OBJ) -o $@ -L. -lmallocator

obj/%.o: %.c header.h
	@mkdir -p obj
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf obj/

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re malloc_test