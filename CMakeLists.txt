cmake_minimum_required(VERSION 4.2)
project(RidiculoEngine)

set(CMAKE_CXX_STANDARD 20)


set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)


add_subdirectory(external/glfw)

add_executable(RidiculoEngine main.cpp external/glad/src/glad.c
        Shader.cpp
        Shader.h)

target_include_directories(RidiculoEngine PRIVATE external/glad/include)
target_link_libraries(RidiculoEngine PRIVATE glfw)