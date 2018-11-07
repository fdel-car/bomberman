#!/bin/bash
if [ ! -d "glfw-3.2.1" ]; then
    curl -OL https://github.com/glfw/glfw/releases/download/3.2.1/glfw-3.2.1.zip
    unzip glfw-3.2.1.zip
    rm "glfw-3.2.1.zip"
fi
if [ ! -d "build" ]; then
    mkdir "build"
    cd build && cmake .. && make
fi