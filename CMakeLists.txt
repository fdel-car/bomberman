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
  srcs/AEntity.cpp
  srcs/AGameScene.cpp
  srcs/AudioManager.cpp
  srcs/Collider.cpp
  srcs/GameEngine.cpp
  srcs/GameRenderer.cpp
  srcs/LevelOne.cpp
  srcs/Player.cpp
  srcs/Wall.cpp
  srcs/GUI/GUI.cpp
)

include_directories(includes)
set(HEADERS
  includes/glad/glad.h
  includes/KHR/khrplatform.h
  includes/nuklear/nuklear.h
  includes/glm/glm.hpp
  includes/glm/gtc/matrix_transform.hpp
  includes/nuklear_glfw_gl3.h
  includes/header.hpp
  includes/AEntity.hpp
  includes/AGameScene.hpp
  includes/AudioManager.hpp
  includes/Collider.hpp
  includes/GameEngine.hpp
  includes/GameRenderer.hpp
  includes/LevelOne.hpp
  includes/Player.hpp
  includes/Wall.hpp
  includes/GUI/GUI.hpp
)

add_executable(super_bomberman ${SRCS} ${HEADERS})

if(MSVC)
  target_compile_options(glfw PRIVATE /W2)
  target_compile_options(super_bomberman PRIVATE /W4 /WX)
else()
  target_compile_options(glfw PRIVATE -Wno-deprecated)
  target_compile_options(super_bomberman PRIVATE -Wall -Wextra -Werror)
endif()

target_compile_features(super_bomberman PRIVATE cxx_std_11)

set(SFML_DIR "${CMAKE_SOURCE_DIR}/SFML-2.5.1/build")
find_package(SFML 2.5.1 COMPONENTS audio REQUIRED)

target_link_libraries(super_bomberman glfw sfml-audio)
