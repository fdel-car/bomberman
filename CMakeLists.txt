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

  srcs/engine/Entity.cpp
  srcs/engine/AudioManager.cpp
  srcs/engine/Collider.cpp
  srcs/engine/AGame.cpp
  srcs/engine/GameEngine.cpp
  srcs/engine/GameRenderer.cpp
  srcs/engine/ShaderProgram.cpp
  srcs/engine/Camera.cpp
  srcs/engine/Light.cpp
  srcs/engine/Model.cpp
  srcs/engine/Mesh.cpp

  srcs/engine/GUI/GUI.cpp

  srcs/game/Bomberman.cpp
  srcs/game/Save.cpp
  srcs/game/scenes/MainMenu.cpp
  srcs/game/scenes/Forest.cpp
  srcs/game/entities/Player.cpp
  srcs/game/entities/Bomb.cpp
  srcs/game/entities/AEnemy.cpp
  srcs/game/entities/EnemyOFDT.cpp
  srcs/game/entities/Explosion.cpp
  srcs/game/entities/Box.cpp
  srcs/game/scenes/SceneTools.cpp
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
  includes/json/json.hpp

  includes/engine/Engine.hpp
  includes/engine/Entity.hpp
  includes/engine/AudioManager.hpp
  includes/engine/Collider.hpp
  includes/engine/AGame.hpp
  includes/engine/GameEngine.hpp
  includes/engine/GameRenderer.hpp
  includes/engine/ShaderProgram.hpp
  includes/engine/Camera.hpp
  includes/engine/Light.hpp
  includes/engine/Model.hpp
  includes/engine/Mesh.hpp

  includes/engine/GUI/GUI.hpp

  includes/game/Bomberman.hpp
  includes/game/Save.hpp
  includes/game/scenes/MainMenu.hpp
  includes/game/scenes/Forest.hpp
  includes/game/entities/Player.hpp
  includes/game/entities/Bomb.hpp
  includes/game/entities/AEnemy.hpp
  includes/game/entities/EnemyOFDT.hpp
  includes/game/entities/Explosion.hpp
  includes/game/entities/Box.hpp
  includes/game/scenes/SceneTools.hpp
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
