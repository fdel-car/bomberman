#!/bin/bash
if [ ! -d "glfw-3.2.1" ]; then
    curl -OL https://github.com/glfw/glfw/releases/download/3.2.1/glfw-3.2.1.zip
    unzip glfw-3.2.1.zip
    rm "glfw-3.2.1.zip"
fi
which -s python || (echo "You need to have python installed first, you can use 'brew install python' for instance." && exit)
which -s pip || (curl https://bootstrap.pypa.io/get-pip.py -o get-pip.py && python get-pip.py --user && export PATH=~/Library/Python/2.7/bin:$PATH)
which -s glad || (pip install glad --user)
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