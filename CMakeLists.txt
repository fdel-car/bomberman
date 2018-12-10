cmake_minimum_required(VERSION 3.0)
project(SuperBomberman)

set(GLFW_BUILD_DOCS OFF)
set(GLFW_BUILD_TESTS OFF)
set(GLFW_BUILD_EXAMPLES OFF)
add_subdirectory(libs/glfw-3.2.1)

set(CMAKE_BUILD_TYPE "Release")
set(SFML_BUILD_DOC OFF)
set(SFML_BUILD_GRAPHICS OFF)
set(SFML_BUILD_WINDOW OFF)
set(SFML_BUILD_NETWORK OFF)
set(SFML_BUILD_AUDIO ON)
set(SFML_BUILD_EXAMPLES OFF)
add_subdirectory(libs/SFML-2.5.1)

set(ASSIMP_BUILD_TESTS OFF)
add_subdirectory(libs/assimp-4.1.0)

include_directories("scrs")
set(SRCS
  srcs/main.cpp

  libs/srcs/glad/glad.cpp

  srcs/engine/Entity.cpp
  srcs/engine/AudioManager.cpp
  srcs/engine/Collider.cpp
  srcs/engine/AGame.cpp
  srcs/engine/GameEngine.cpp
  srcs/engine/GameRenderer.cpp
  srcs/engine/ShaderProgram.cpp
  srcs/engine/Joint.cpp
  srcs/engine/Camera.cpp
  srcs/engine/Light.cpp
  srcs/engine/Model.cpp
  srcs/engine/Mesh.cpp
  srcs/engine/Skybox.cpp
  srcs/engine/GUI/GUI.cpp

  srcs/game/Bomberman.cpp
  srcs/game/Save.cpp
  srcs/game/scenes/SceneTools.cpp
  srcs/game/scenes/MainMenu.cpp
  srcs/game/scenes/Forest.cpp
  srcs/game/scenes/Pokemon.cpp
  srcs/game/scenes/Load.cpp
  srcs/game/scenes/Volcano.cpp
  srcs/game/scenes/Space.cpp
  srcs/game/scenes/Credits.cpp
  srcs/game/entities/Player.cpp
  srcs/game/entities/Bomb.cpp
  srcs/game/entities/AEnemy.cpp
  srcs/game/entities/EnemyBasic.cpp
  srcs/game/entities/EnemyRunAway.cpp
  srcs/game/entities/EnemyMelee.cpp
  srcs/game/entities/Explosion.cpp
  srcs/game/entities/Box.cpp
  srcs/game/entities/Damageable.cpp
  srcs/game/entities/Perk.cpp
  srcs/game/entities/Portal.cpp
)

include_directories("includes")
include_directories("libs/includes")
include_directories("libs/assimp-4.1.0/include" "build/libs/assimp-4.1.0/include")
set(HEADERS
  libs/includes/glad/glad.h
  libs/includes/KHR/khrplatform.h
  libs/includes/nuklear/nuklear.h
  libs/includes/glm/glm.hpp
  libs/includes/glm/gtc/matrix_transform.hpp
  libs/includes/glm/gtc/type_ptr.hpp
  libs/includes/json/json.hpp

  includes/engine/Engine.hpp
  includes/engine/Entity.hpp
  includes/engine/AudioManager.hpp
  includes/engine/Collider.hpp
  includes/engine/AGame.hpp
  includes/engine/GameEngine.hpp
  includes/engine/GameRenderer.hpp
  includes/engine/ShaderProgram.hpp
  includes/engine/Joint.hpp
  includes/engine/Camera.hpp
  includes/engine/Light.hpp
  includes/engine/Model.hpp
  includes/engine/Mesh.hpp
  includes/engine/Skybox.hpp
  includes/engine/GUI/GUI.hpp

  includes/game/Bomberman.hpp
  includes/game/Save.hpp
  includes/game/scenes/MainMenu.hpp
  includes/game/scenes/Forest.hpp
  includes/game/scenes/Pokemon.hpp
  includes/game/scenes/Load.hpp
  includes/game/scenes/Volcano.hpp
  includes/game/scenes/Space.hpp
  includes/game/scenes/Credits.hpp
  includes/game/entities/Player.hpp
  includes/game/entities/Bomb.hpp
  includes/game/entities/AEnemy.hpp
  includes/game/entities/EnemyBasic.hpp
  includes/game/entities/EnemyRunAway.hpp
  includes/game/entities/EnemyMelee.hpp
  includes/game/entities/Explosion.hpp
  includes/game/entities/Box.hpp
  includes/game/entities/Damageable.hpp
  includes/game/entities/Perk.hpp
  includes/game/entities/Portal.hpp
  includes/game/scenes/SceneTools.hpp
)

add_executable(super_bomberman ${SRCS} ${HEADERS})

if(MSVC)
  target_compile_options(glfw PRIVATE /W2)
  target_compile_options(sfml-audio PRIVATE /W2)
  target_compile_options(assimp PRIVATE /W2)
  target_compile_options(super_bomberman PRIVATE /W4 /WX)
else()
  target_compile_options(glfw PRIVATE -Wno-deprecated -Wno-macro-redefined)
  target_compile_options(sfml-audio PRIVATE -Wno-deprecated)
  target_compile_options(assimp PRIVATE -Wno-deprecated -Wno-delete-non-virtual-dtor -Wno-address-of-packed-member -Wno-unused-private-field)
  target_compile_options(super_bomberman PRIVATE -Wall -Wextra -Werror)
endif()
target_compile_features(super_bomberman PRIVATE cxx_std_11)
target_link_libraries(super_bomberman glfw sfml-audio assimp)
