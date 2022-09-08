CFLAGS = 
# CFLAGS = -Wall -Wextra -Werror
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

re: fclean all