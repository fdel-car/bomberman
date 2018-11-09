#!/bin/bash
if [ ! -d "glfw-3.2.1" ]; then
    curl -OL https://github.com/glfw/glfw/releases/download/3.2.1/glfw-3.2.1.zip
    unzip glfw-3.2.1.zip > /dev/null
    rm "glfw-3.2.1.zip"
fi
pythonVersion="$(python -c 'import sys; print(".".join(map(str, sys.version_info[:2])))')"
which -s python || (echo "You need to have python installed first, you can use 'brew install python' for instance." && exit)
which -s pip
if [ $? == 1 ]; then
    curl https://bootstrap.pypa.io/get-pip.py -o get-pip.py
    python get-pip.py --user
    rm get-pip.py
    echo -e "\033[0;33mNow that pip is installed please note that you should add it to your path.\n\
    So the commands pip and glad won't work for you as it, but that line: 'export PATH=~/Library/Python/${pythonVersion}/bin:\$PATH' in your .bashrc will do the trick.\033[0m"
fi
which -s glad || (~/Library/Python/${pythonVersion}/bin/pip install glad --user)
if [[ ! -d "srcs/glad" && ! -d "includes/glad" && ! -d "includes/KHR" ]]; then
    ~/Library/Python/${pythonVersion}/bin/glad --api gl=4.1 --profile=core --generator=c --out-path=tmp-glad
    mkdir srcs/glad && mv tmp-glad/src/glad.c srcs/glad/glad.cpp
    mv tmp-glad/include/KHR includes/ && mv tmp-glad/include/glad includes/
    rm -rf tmp-glad
fi
if [ ! -d "includes/nuklear" ]; then
    mkdir includes/nuklear
    curl https://raw.githubusercontent.com/vurtun/nuklear/master/nuklear.h -o includes/nuklear/nuklear.h
fi
which -s cmake || (echo "Without cmake installed on your computer you can't fully finish the installation. The command 'brew install cmake' can be pretty useful!" && exit)
if [ ! -d "SFML-2.5.1" ]; then
    curl -OL https://www.sfml-dev.org/files/SFML-2.5.1-sources.zip
    unzip SFML-2.5.1-sources.zip > /dev/null
    rm "SFML-2.5.1-sources.zip"
    mkdir SFML-2.5.1/build && cd SFML-2.5.1/build
    cmake -DSFML_BUILD_EXAMPLES=OFF -DSFML_BUILD_DOC=OFF -DCMAKE_BUILD_TYPE:STRING=Release -DSFML_BUILD_GRAPHICS=OFF -DSFML_BUILD_WINDOW=OFF -DSFML_BUILD_NETWORK=OFF -SFML_BUILD_AUDIO=ON ..
    make && cd ../..
fi
if [ ! -d "build" ]; then
    mkdir "build"
    cd build && cmake .. && make
fi