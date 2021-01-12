cmake_minimum_required(VERSION 3.3)
project(Iron)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(SOURCE_FILES source/Iron/Rendering/Shader.h source/Iron/Rendering/Shader.cpp includes/glad/src/glad.c source/Iron/Core/Application.cpp source/Iron/Core/Application.h source/Iron/Core/Input.cpp source/Iron/Core/Input.h source/Iron/Core/KeyCodes.h source/Iron/Core/MouseCodes.h source/Iron.h source/Iron/Scene/Object.cpp source/Iron/Scene/Object.h source/Iron/Scene/Component.cpp source/Iron/Scene/Component.h source/Iron/Rendering/SpriteRenderer.cpp source/Iron/Rendering/SpriteRenderer.h source/Iron/Scene/Scene.cpp source/Iron/Scene/Scene.h source/Iron/Scene/Transformation.cpp source/Iron/Scene/Transformation.h source/Iron/Core/Log.cpp source/Iron/Core/Log.h source/Iron/Rendering/Camera.cpp source/Iron/Rendering/Camera.h source/Iron/Core/Time.cpp source/Iron/Core/Time.h source/Iron/Rendering/Screen.cpp source/Iron/Rendering/Screen.h)

add_definitions(-DGLEW_STATIC)

include_directories(includes/stb/include)
include_directories(includes/glm/include)
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