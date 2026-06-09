NAME        = codexion

SRC_DIR     = coders/
INC_DIR     = coders/
OBJ_DIR     = obj

FILES       = \
          clean.c\
          clean_one.c\
          coder_state.c\
          create_thread.c\
          dongle_utils.c\
          error.c\
          handle_priority.c\
          init_struct.c\
          init_utils.c\
          main.c\
          monitor.c\
          monitor_utils.c\
          parser.c\
          thread_utils.c\
          time_keeper.c

SRCS        = $(addprefix $(SRC_DIR)/, $(FILES))
OBJS        = $(addprefix $(OBJ_DIR)/, $(FILES:.c=.o))
DEPS        = $(addprefix $(OBJ_DIR)/, $(FILES:.c=.d))

CC          = cc
CFLAGS      = -Wall -Wextra -Werror -MMD -I$(INC_DIR) -pthread

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c Makefile
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

-include $(DEPS)

.PHONY: all clean fclean re