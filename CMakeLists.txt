cmake_minimum_required(VERSION 3.3)
project(Engine)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(SOURCE_FILES source/main.cpp source/renderer/shader.h source/renderer/shader.cpp)

add_definitions(-DGLEW_STATIC)

include_directories("includes")

include_directories(includes/glfw/include)
link_directories(includes/glfw/lib)

include_directories(includes/glew/include)
link_directories(includes/glew/lib)

add_executable(${CMAKE_PROJECT_NAME} ${SOURCE_FILES})

target_link_libraries(${CMAKE_PROJECT_NAME} glfw3 glew32s gdi32 opengl32)