cmake_minimum_required(VERSION 3.16)
project(RevRenderEngine VERSION 0.1 LANGUAGES C CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(glfw3 QUIET)
if(NOT glfw3_FOUND)
    message(STATUS "GLFW NOT FOUND. BUILDING FROM SOURCE")
    set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    add_subdirectory(external/glfw)
endif()

find_package(assimp QUIET)
if(NOT assimp_FOUND)
    message(STATUS "ASSIMP NOT FOUND. BUILDING FROM SOURCE")
    set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    add_subdirectory(external/assimp)
endif()

add_library(glm INTERFACE)
target_include_directories(glm INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/external/glm)

add_subdirectory(external)
add_subdirectory(engine)
add_subdirectory(backend)
add_subdirectory(revrender)
