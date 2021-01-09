cmake_minimum_required(VERSION 3.3)
project(Iron)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(SOURCE_FILES source/Iron/Shader.h source/Iron/Shader.cpp includes/glad/src/glad.c source/Iron/Application.cpp source/Iron/Application.h source/Iron/Input.cpp source/Iron/Input.h source/Iron/KeyCodes.h source/Iron/MouseCodes.h source/Iron.h source/Iron/Object.cpp source/Iron/Object.h source/Iron/Component.cpp source/Iron/Component.h source/Iron/SpriteRenderer.cpp source/Iron/SpriteRenderer.h source/Iron/Scene.cpp source/Iron/Scene.h source/Iron/Transformation.cpp source/Iron/Transformation.h source/Iron/Log.cpp source/Iron/Log.h source/Iron/Camera.cpp source/Iron/Camera.h source/Iron/Time.cpp source/Iron/Time.h source/Iron/Screen.cpp source/Iron/Screen.h)

add_definitions(-DGLEW_STATIC)

include_directories("includes/stb/include")
include_directories("includes/glm/include")
include_directories(includes/glad/include)
include_directories(includes/glfw/include)

link_directories(includes/glfw/lib)

add_library(${CMAKE_PROJECT_NAME} SHARED ${SOURCE_FILES})

target_link_libraries(${CMAKE_PROJECT_NAME} glfw3 gdi32 opengl32 "-static")

#For sandbox testing purposes
add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${CMAKE_PROJECT_NAME}>
        ${PROJECT_SOURCE_DIR}/../Sandbox/includes/Iron)
add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/source/Iron
        ${PROJECT_SOURCE_DIR}/../Sandbox/includes/Iron/Iron)
add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/source/Iron.h
        ${PROJECT_SOURCE_DIR}/../Sandbox/includes/Iron)