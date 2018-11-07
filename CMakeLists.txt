cmake_minimum_required(VERSION 3.12.4)
project(Bomberman)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(glfw-3.2.1)

set(SRCS
    main.cpp
)

add_executable(game ${SRCS})

if(MSVC)
  target_compile_options(glfw PRIVATE /W2)
  target_compile_options(game PRIVATE /W4 /WX)
else()
  target_compile_options(glfw PRIVATE -Wno-deprecated)
  target_compile_options(game PRIVATE -Wall -Wextra -Werror -std=c++11)
endif()

target_link_libraries(game glfw)