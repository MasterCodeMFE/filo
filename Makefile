# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: manufern <manufern@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/15 10:39:24 by manufern          #+#    #+#              #
#    Updated: 2024/10/15 10:55:02 by manufern         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo

OBJDIR = ./obj

SRC = ./src/main.c \
	  ./src/ft_atoi.c \
	  ./src/ft_usleep.c \
	  ./src/create_threads.c \
	  ./src/eat.c \
	  ./src/monitoring.c \
	  ./src/routine.c \
	  ./src/print_sleep.c \
	  ./src/sleep.c \
	  ./src/init_mutex.c \
	  ./src/take_fork.c \
	  ./src/think.c \
	  ./src/time.c

OBJ = $(SRC:./src/%.c=$(OBJDIR)/%.o)

CC = gcc
CFLAGS = -Wall -Werror -Wextra

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME) $(LIBS)

$(OBJDIR)/%.o: ./src/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean:
	rm -rf $(OBJDIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re