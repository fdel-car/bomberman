cmake_minimum_required(VERSION 3.12.4)
project(Bomberman)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(glfw-3.2.1)

include_directories(scrs)
set(SRCS
  srcs/main.cpp
  srcs/glad/glad.cpp
  srcs/AudioManager.cpp
  srcs/GameLogic.cpp
  srcs/GameRenderer.cpp
)

include_directories(includes)
set(HEADERS
  includes/glad/glad.h
  includes/KHR/khrplatform.h
  includes/nuklear/nuklear.h
  includes/nuklear_glfw_gl3.h
  includes/header.hpp
  includes/AudioManager.hpp
  includes/GameLogic.hpp
  includes/GameRenderer.hpp
)

add_executable(bomberman ${SRCS} ${HEADERS})

if(MSVC)
  target_compile_options(glfw PRIVATE /W2)
  target_compile_options(bomberman PRIVATE /W4 /WX)
else()
  target_compile_options(glfw PRIVATE -Wno-deprecated)
  target_compile_options(bomberman PRIVATE -Wall -Wextra -Werror -std=c++11)
endif()

target_link_libraries(bomberman glfw)