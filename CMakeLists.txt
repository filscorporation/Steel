cmake_minimum_required(VERSION 3.3)
project(Iron)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(GLAD_FILES includes/glad/src/glad.c)
set(SOURCE_FILES
        ${GLAD_FILES}
        source/Iron/Rendering/Shader.h source/Iron/Rendering/Shader.cpp source/Iron/Core/Application.cpp
        source/Iron/Core/Application.h source/Iron/Core/Input.cpp source/Iron/Core/Input.h source/Iron/Core/KeyCodes.h
        source/Iron/Core/MouseCodes.h source/Iron.h source/Iron/Scene/Object.cpp source/Iron/Scene/Object.h
        source/Iron/Scene/Component.cpp source/Iron/Scene/Component.h source/Iron/Rendering/SpriteRenderer.cpp
        source/Iron/Rendering/SpriteRenderer.h source/Iron/Scene/Scene.cpp source/Iron/Scene/Scene.h
        source/Iron/Scene/Transformation.cpp source/Iron/Scene/Transformation.h source/Iron/Core/Log.cpp
        source/Iron/Core/Log.h source/Iron/Rendering/Camera.cpp source/Iron/Rendering/Camera.h source/Iron/Core/Time.cpp
        source/Iron/Core/Time.h source/Iron/Rendering/Screen.cpp source/Iron/Rendering/Screen.h
        source/Iron/Core/ResourcesManager.cpp source/Iron/Core/ResourcesManager.h source/Iron/Rendering/Renderer.cpp
        source/Iron/Rendering/Renderer.h source/Iron/Animation/Animator.cpp source/Iron/Animation/Animator.h
        source/Iron/Animation/Animation.cpp source/Iron/Animation/Animation.h source/Iron/Rendering/Sprite.cpp
        source/Iron/Rendering/Sprite.h)

cmake_policy(SET CMP0072 NEW)

# project target
add_library(Iron ${SOURCE_FILES})

# opengl
find_package(OpenGL REQUIRED)
target_link_libraries(Iron PRIVATE ${OPENGL_LIBRARIES})

# stb
include_directories(includes)

# glad
include_directories(includes/glad/include)

# glfw
add_subdirectory(includes/glfw EXCLUDE_FROM_ALL)
target_link_libraries(Iron PRIVATE glfw)

# glm
add_subdirectory(includes/glm EXCLUDE_FROM_ALL)
target_link_libraries(Iron PRIVATE glm)
