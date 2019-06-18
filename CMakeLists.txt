cmake_minimum_required(VERSION 3.10)

# On Windows, these variables need to be customized for your environment.  
# On Linux you can apt-get install these to system locations
if(WIN32)
    set(SDL2_DIR          "/Users/rallen/Documents/Devel/SDL2-2.0.9")
    set(GLEW_INCLUDE_DIR  "/Users/rallen/Documents/Devel/glew-2.1.0/include")
    set(GLEW_LIBRARY      "/Users/rallen/Documents/Devel/glew-2.1.0/lib/Release/x64/glew32.lib")
    set(GLEW_DLL_DIR      "/Users/rallen/Documents/Devel/glew-2.1.0/bin/Release/x64")
endif()

include(CheckLanguage)
check_language(CUDA)

project(hello
    VERSION     0.1
    DESCRIPTION "CUDA, OpenGL & SDL starting point"
    LANGUAGES   CUDA CXX
)

add_executable(hello hello.cu main.cpp)

# CUDA wants C++11
if(NOT DEFINED CMAKE_CUDA_STANDARD)
    set(CMAKE_CUDA_STANDARD 11)
    set(CMAKE_CUDA_STANDARD_REQUIRED ON)
endif()

# Windows prereq: get dev libraries from https://www.libsdl.org/download-2.0.php
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
# workaround for CMP0004 issue with the SDL2 libraries on Linux
string(STRIP "${SDL2_LIBRARIES}" SDL2_LIBRARIES)
target_link_libraries(hello ${SDL2_LIBRARIES})

# OpenGL
set(OpenGL_GL_PREFERENCE "GLVND")
find_package(OpenGL REQUIRED COMPONENTS OpenGL)
if(NOT TARGET OpenGL::OpenGL)
  target_link_libraries(hello OpenGL::GL)
  target_include_directories(hello PUBLIC OpenGL::GL)
else()
  target_link_libraries(hello OpenGL::OpenGL)
  target_include_directories(hello PUBLIC OpenGL::OpenGL)
endif()

# GL Extension Wrangler (GLEW) get dev libraries from http://glew.sourceforge.net/
find_package(GLEW REQUIRED)
target_link_libraries(hello "${GLEW_LIBRARIES}")
target_include_directories(hello PUBLIC "${GLEW_INCLUDE_DIRS}")

# Windows: copy the dll as a post-build step
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