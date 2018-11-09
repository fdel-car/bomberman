cmake_minimum_required(VERSION 3.0)
project(SuperBomberman)

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

set(SFML_DIR "${CMAKE_SOURCE_DIR}/SFML-2.5.1/build")
find_package(SFML 2.5.1 COMPONENTS audio REQUIRED)

add_executable(super_bomberman ${SRCS} ${HEADERS})

if(MSVC)
  target_compile_options(glfw PRIVATE /W2)
  target_compile_options(super_bomberman PRIVATE /W4 /WX)
else()
  target_compile_options(glfw PRIVATE -Wno-deprecated)
  target_compile_options(super_bomberman PRIVATE -Wall -Wextra -Werror)
endif()

target_compile_features(super_bomberman PRIVATE cxx_std_11)

target_link_libraries(super_bomberman glfw sfml-audio)