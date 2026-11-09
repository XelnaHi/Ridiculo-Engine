cmake_minimum_required(VERSION 4.2)
project(RidiculoEngine)

set(CMAKE_CXX_STANDARD 20)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(external/glfw)

# --- Assimp ---
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "" FORCE)
set(ASSIMP_INSTALL OFF CACHE BOOL "" FORCE)
set(ASSIMP_NO_EXPORT ON CACHE BOOL "" FORCE)
set(ASSIMP_WARNINGS_AS_ERRORS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_OBJ_IMPORTER ON CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_FBX_IMPORTER ON CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_GLTF_IMPORTER ON CACHE BOOL "" FORCE)
add_subdirectory(external/assimp)
# --------------

add_executable(RidiculoEngine main.cpp external/glad/src/glad.c
        Shader.cpp
        Shader.h
        stb/stb_image.h
        stb/stb_image.cpp
        math_3d.h
        generate_sphere_fixed_ccw.cpp)

target_include_directories(RidiculoEngine PRIVATE external/glad/include external)
target_link_libraries(RidiculoEngine PRIVATE glfw assimp::assimp)
