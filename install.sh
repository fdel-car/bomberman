#!/bin/bash
if [ ! -d "glfw-3.2.1" ]; then
    curl -OL https://github.com/glfw/glfw/releases/download/3.2.1/glfw-3.2.1.zip
    unzip glfw-3.2.1.zip
    rm "glfw-3.2.1.zip"
fi
PYTHON_VERSION="$(python -c 'import sys; print(".".join(map(str, sys.version_info[:2])))')"
which -s python || (echo "You need to have python installed first, you can use 'brew install python' for instance." && exit)
which -s pip
if [ $? == 1 ]; then
    curl https://bootstrap.pypa.io/get-pip.py -o get-pip.py
    python get-pip.py --user
    rm get-pip.py
    echo -e "\033[0;33mNow that pip is installed please note that you should add it to your path.\n'export PATH=~/Library/Python/${PYTHON_VERSION}/bin:\$PATH' in your .bashrc could do the trick.\033[0m"
fi
which -s glad || (~/Library/Python/${PYTHON_VERSION}/bin/pip install glad --user)
if [[ ! -d "srcs/glad" && ! -d "includes/glad" && ! -d "includes/KHR" ]]; then
    glad --api gl=4.1 --profile=core --generator=c --out-path=tmp-glad
    mkdir srcs/glad && mv tmp-glad/src/glad.c srcs/glad/
    mv tmp-glad/include/KHR includes/ && mv tmp-glad/include/glad includes/
    rm -rf tmp-glad
fi
if [ ! -d "build" ]; then
    mkdir "build"
    cd build && cmake .. && make
fi