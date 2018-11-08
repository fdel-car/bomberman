NAME = bomberman

SDIR_MAIN = ./srcs_main/

INCLUDES_DIR = includes

SRC_MAIN = $(SDIR_MAIN)AudioManager.cpp	$(SDIR_MAIN)GlfwGUI.cpp $(SDIR_MAIN)MainGame.cpp

OBJ_MAIN = $(SRC_MAIN:.cpp=.o)

ALL = $(NAME)

CFLAGS = -Wall -Wextra -Werror -ferror-limit=2 -I $(INCLUDES_DIR) -std=c++11

GLFW_FLAGS = $(DLFLAGS) `pkg-config --libs glfw3` -framework OpenGL

AUDIO_FLAGS = $(DLFLAGS) `pkg-config --libs sfml-audio`

ALL_FLAGS = $(CFLAGS) $(GLFW_FLAGS) $(AUDIO_FLAGS)

CC = clang++

all: $(ALL)

install:
	brew install pkg-config
	brew install glfw
	cp -r ~/.brew/Cellar/glfw/3.2.1/include/GLFW ./$(INCLUDES_DIR)
	brew install sfml
	mkdir ./$(INCLUDES_DIR)/SFML
	cp -r ~/.brew/Cellar/sfml/2.4.2_1/include/SFML/*.hpp ~/.brew/Cellar/sfml/2.4.2_1/include/SFML/Audio ~/.brew/Cellar/sfml/2.4.2_1/include/SFML/System ./$(INCLUDES_DIR)/SFML
	export LD_LIBRARY_PATH=~/.brew/Cellar/sfml/2.4.2_1/lib
	echo "export LD_LIBRARY_PATH=~/.brew/Cellar/sfml/2.4.2_1/lib" >> ~/.zshrc

$(NAME): $(OBJ_MAIN)
	$(CC) $(ALL_FLAGS) -o $(NAME) $(OBJ_MAIN)
	@echo "				$(NAME) created"

.cpp.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@/bin/rm -f $(OBJ_MAIN)
	@echo "				All project's .o deleted"

fclean: clean
	@/bin/rm -f $(ALL)
	@echo "				Exec deleted"

re: fclean all

.PHONY: all lib clean fclean re
