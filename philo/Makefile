CFLAGS = -Wall -Wextra -Werror -pthread
CPPFLAGS = -I includes
NAME = philo
SRCS = main.c utility.c th_monitor.c th_philo.c init.c
OBJ = $(addprefix $(OBJDIR)/, $(SRCS:%.c=%.o))
OBJDIR = obj
VPATH = srcs

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) -o $(NAME) $(OBJ) 

$(OBJDIR)/%.o: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -o $@ -c $<

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

leak:
	@make re
	leaks -q -atExit -- ./$(NAME) 5 800 200 200 5
	@make fclean

re: fclean all