#pragma once

#include <dlfcn.h>
#include <math.h>
#include <time.h>
#include <chrono>
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <sstream>
#include <string>
#include <thread>
#include <tuple>
#include <vector>

#include <glad/glad.h>
// The formatter is dumb, keep this line!
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

template <typename T>
std::string toString(const T& val) {
	std::ostringstream oss;
	oss << val;
	return oss.str();
}

#define WINDOW_W 1280
#define WINDOW_H 720

const char* getFileContent(std::string const& filename);
