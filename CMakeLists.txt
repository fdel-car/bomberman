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
  srcs/Entity.cpp
  srcs/AGameScene.cpp
  srcs/AudioManager.cpp
  srcs/Collider.cpp
  srcs/GameEngine.cpp
  srcs/GameRenderer.cpp
  srcs/ShaderProgram.cpp
  srcs/Camera.cpp
  srcs/Shape.cpp
  srcs/Player.cpp
  srcs/GUI/GUI.cpp
  srcs/AGame.cpp
  srcs/scenes/BomberManGame.cpp
  srcs/scenes/MainMenu.cpp
  srcs/scenes/MainMenuCam.cpp
  srcs/scenes/Level01.cpp
  srcs/scenes/Level01Cam.cpp
)

include_directories(includes)
set(HEADERS
  includes/glad/glad.h
  includes/KHR/khrplatform.h
  includes/nuklear/nuklear.h
  includes/tiny_obj_loader/tiny_obj_loader.h
  includes/glm/glm.hpp
  includes/glm/gtc/matrix_transform.hpp
  includes/glm/gtc/type_ptr.hpp
  includes/header.hpp
  includes/Entity.hpp
  includes/AGameScene.hpp
  includes/AudioManager.hpp
  includes/Collider.hpp
  includes/GameEngine.hpp
  includes/GameRenderer.hpp
  includes/ShaderProgram.hpp
  includes/Camera.hpp
  includes/Shape.hpp
  includes/Player.hpp
  includes/GUI/GUI.hpp
  includes/AGame.hpp
  includes/scenes/BomberManGame.hpp
  includes/scenes/MainMenu.hpp
  includes/scenes/MainMenuCam.hpp
  includes/scenes/Level01.hpp
  includes/scenes/Level01Cam.hpp
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
