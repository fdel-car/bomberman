# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jichen-m <jichen-m@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/11/07 11:48:44 by jichen-m          #+#    #+#              #
#    Updated: 2018/11/07 19:18:38 by jichen-m         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = TestCmake

LIB = ./includes/

C_FILES = main.cpp

FLAGS = -Wall -Wextra -Werror -I $(LIB) -std=c++11

# GCC_LIBS = -framework AppKit -framework OpenGL -framework IOKit -framework CoreVideo

GLFW3 = $(FLAGS) `pkg-config --libs glfw3` -framework OpenGL

CC = clang++

SRC = $(addprefix ./srcs/,$(C_FILES))
LIBI = $(addprefix -I, $(LIB))
LIBL = $(addprefix -L, $(LIB))

OBJ = $(SRC:.c=.o)

all: $(NAME)

install:
	brew install cmake
	brew install pkg-config
	brew install glfw3
	cp -r ~/.brew/Cellar/glfw/3.2.1/include/ ./includes

uninstall:
	brew uninstall pkg-config
	brew uninstall glfw3
	rm -rf ./includes/GLWF

$(NAME): $(OBJ)
		@echo "\033[1;32m"
		@echo "Processing files..."
		@$(CC) $(GLFW3) $(OBJ) -o $(NAME)
		@echo "\033[1;32m"
		@echo "$(NAME) has been created! :D"

%.o: %.c
		@$(CC) $(FLAGS) -o $@ -c $< $(LIBI)

clean:
		@rm -rf $(NAME)
		@echo "\033[1;32mObjects files have been removed successfully!"

fclean: clean
		@echo "\033[1;32m$(NAME) has been removed! All clean ^^!"

re: fclean all

.PHONY: all clean fclean re