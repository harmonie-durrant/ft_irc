SRC =	src/main.cpp		\
		#src/server.cpp		\

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

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean $(NAME)

.PHONY: all clean fclean re