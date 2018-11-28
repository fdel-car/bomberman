#!/bin/bash

# Create libs directory if needed
if [ ! -d "libs" ]; then
    mkdir libs
fi

# GLFW 3.2.1 install
if [ ! -d "libs/glfw-3.2.1" ]; then
    curl -OL https://github.com/glfw/glfw/releases/download/3.2.1/glfw-3.2.1.zip
    unzip glfw-3.2.1.zip > /dev/null
    mv glfw-3.2.1 libs/
    rm "glfw-3.2.1.zip"
fi

which python >> /dev/null || (echo "You need to have python installed first, you can use 'brew install python' for instance." || exit 0)
# pip install
which pip >> /dev/null
if [ $? == 1 ]; then
    curl https://bootstrap.pypa.io/get-pip.py -o get-pip.py
    python get-pip.py --user
    rm get-pip.py
	echo -e "\033[0;31mIn order to finish the install script you need to add the path to the pip binary to your PATH env variable.\033[0m"
    exit 0;
fi

# GLAD install
which glad >> /dev/null || pip install glad --user
if [[ ! -d "libs/srcs/glad" && ! -d "libs/includes/glad" && ! -d "libs/includes/KHR" ]]; then
    glad --api gl=4.1 --profile=core --generator=c --out-path=tmp-glad
    mkdir -p libs/srcs/glad
    mv tmp-glad/src/glad.c libs/srcs/glad/glad.cpp
    mkdir -p libs/includes/glad
    mv tmp-glad/include/glad/glad.h libs/includes/glad
    mkdir -p libs/includes/KHR
    mv tmp-glad/include/KHR/khrplatform.h libs/includes/KHR
    rm -rf tmp-glad
fi

# GLM 0.9.9.3 install
if [ ! -d "libs/includes/glm" ]; then
	curl -OL https://github.com/g-truc/glm/releases/download/0.9.9.3/glm-0.9.9.3.zip
	unzip glm-0.9.9.3.zip > /dev/null
	rm "glm-0.9.9.3.zip"
	mv glm/glm libs/includes/
	rm -rf glm
fi

# stb_image install
if [ ! -d "libs/includes/stb_image" ]; then
    mkdir -p libs/includes/stb_image
    curl -L -o libs/includes/stb_image/stb_image.h "https://drive.google.com/uc?export=download&id=1u29S1t-cOXgSlJv_e8D8Qjn929VyT2Go"
fi

# Nuklear install
if [ ! -d "libs/includes/nuklear" ]; then
    mkdir -p libs/includes/nuklear
    curl https://raw.githubusercontent.com/vurtun/nuklear/master/nuklear.h -o libs/includes/nuklear/nuklear.h
fi

# Json parser install
if [ ! -d "libs/includes/json" ]; then
    mkdir libs/includes/json
    curl https://raw.githubusercontent.com/nlohmann/json/develop/single_include/nlohmann/json.hpp -o libs/includes/json/json.hpp
fi

which cmake >> /dev/null || (echo "Without cmake installed on your computer you can't fully finish the installation. The command 'brew install cmake' can be pretty useful!" || exit 0)

# SFML 2.5.1 audio install
if [ ! -d "SFML-2.5.1" ]; then
    curl -OL https://www.sfml-dev.org/files/SFML-2.5.1-sources.zip
    unzip SFML-2.5.1-sources.zip > /dev/null
    rm "SFML-2.5.1-sources.zip"
    mkdir SFML-2.5.1/build && cd SFML-2.5.1/build
    cmake -DSFML_BUILD_EXAMPLES=OFF -DSFML_BUILD_DOC=OFF -DCMAKE_BUILD_TYPE:STRING=Release -DSFML_BUILD_GRAPHICS=OFF -DSFML_BUILD_WINDOW=OFF -DSFML_BUILD_NETWORK=OFF -SFML_BUILD_AUDIO=ON ..
    make && cd ../..
fi

# At the end off the first install, build the game
if [ ! -d "build" ]; then
    mkdir "build"
    cd build && cmake .. && make
fi