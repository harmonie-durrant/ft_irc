# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: froque <froque@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/12/10 13:56:09 by froque            #+#    #+#              #
#    Updated: 2024/12/10 13:56:11 by froque           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# pa sutile

SRC =	src/main.cpp				\
		src/Server.cpp				\
		src/Client.cpp				\
		src/Command.cpp				\
		src/commands/Cap.cpp		\
		src/commands/Mode.cpp		\
		src/commands/Nick.cpp		\
		src/commands/Pass.cpp		\
		src/commands/Part.cpp		\
		src/commands/Privmsg.cpp	\
		src/commands/Ping.cpp		\
		src/commands/Quit.cpp		\
		src/commands/User.cpp		\
		src/commands/Whois.cpp		\

OBJ = $(SRC:.cpp=.o)

CC = c++

CPPFLAGS = -Wall -Wextra -Werror -std=c++98

INCLUDE_DIR = ./include

NAME = ircserv

%.o: %.cpp
	$(CC) $(CPPFLAGS) -c $< -o $@ -I$(INCLUDE_DIR)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CPPFLAGS) $(OBJ) -o $(NAME)

check: all
	sh ./check.sh

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean $(NAME)

.PHONY: all clean fclean re
