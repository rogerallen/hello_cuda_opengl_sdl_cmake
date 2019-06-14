cmake_minimum_required(VERSION 3.9)

include("CMakeLocals.txt")

include(CheckLanguage)
check_language(CUDA)

project(hello
    VERSION     0.1
    DESCRIPTION "CUDA, OpenGL & SDL starting point"
    LANGUAGES   CUDA CXX
)

if(NOT DEFINED CMAKE_CUDA_STANDARD)
    set(CMAKE_CUDA_STANDARD 11)
    set(CMAKE_CUDA_STANDARD_REQUIRED ON)
endif()

add_executable(hello hello.cu main.cpp)

# Windows prereq: get dev libraries from https://www.libsdl.org/download-2.0.php
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
target_link_libraries(hello ${SDL2_LIBRARIES})

# OpenGL
find_package(OpenGL REQUIRED)
target_link_libraries(hello ${OPENGL_gl_LIBRARY})
target_include_directories(hello PUBLIC ${OPENGL_INCLUDE_DIR})

# GL Extension Wrangler (GLEW) get dev libraries from http://glew.sourceforge.net/
find_package(GLEW REQUIRED)
target_link_libraries(hello ${GLEW_LIBRARIES})
target_include_directories(hello PUBLIC ${GLEW_INCLUDE_DIRS})

# Windows: get the dll as a post-build step
if(WIN32)
    add_custom_command(TARGET hello POST_BUILD
         COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${SDL2_DIR}/lib/x64/SDL2.dll"
            $<TARGET_FILE_DIR:hello>)
    add_custom_command(TARGET hello POST_BUILD
         COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${GLEW_DLL_DIR}/glew32.dll"
            $<TARGET_FILE_DIR:hello>)
endif()