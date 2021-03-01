cmake_minimum_required(VERSION 3.3)
project(Iron VERSION 0.0.1)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif()

set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(GLAD_FILES includes/glad/src/glad.c)
set(INCLUDE_FOLDER "includes")
set(SOURCE_FILES
        ${GLAD_FILES}
        source/Iron/Rendering/Shader.h source/Iron/Rendering/Shader.cpp source/Iron/Core/Application.cpp
        source/Iron/Core/Application.h source/Iron/Core/Input.cpp source/Iron/Core/Input.h source/Iron/Core/KeyCodes.h
        source/Iron/Core/MouseCodes.h source/Iron.h source/Iron/Scene/Entity.cpp source/Iron/Scene/Entity.h
        source/Iron/Scene/Component.cpp source/Iron/Scene/Component.h source/Iron/Rendering/SpriteRenderer.cpp
        source/Iron/Rendering/SpriteRenderer.h source/Iron/Scene/Scene.cpp source/Iron/Scene/Scene.h
        source/Iron/Scene/Transformation.cpp source/Iron/Scene/Transformation.h source/Iron/Core/Log.cpp
        source/Iron/Core/Log.h source/Iron/Rendering/Camera.cpp source/Iron/Rendering/Camera.h source/Iron/Core/Time.cpp
        source/Iron/Core/Time.h source/Iron/Rendering/Screen.cpp source/Iron/Rendering/Screen.h
        source/Iron/Core/ResourcesManager.cpp source/Iron/Core/ResourcesManager.h source/Iron/Rendering/Renderer.cpp
        source/Iron/Rendering/Renderer.h source/Iron/Animation/Animator.cpp source/Iron/Animation/Animator.h
        source/Iron/Animation/Animation.cpp source/Iron/Animation/Animation.h source/Iron/Rendering/Sprite.cpp
        source/Iron/Rendering/Sprite.h source/Iron/Audio/AudioTrack.cpp source/Iron/Audio/AudioTrack.h
        source/Iron/Audio/AudioSource.cpp source/Iron/Audio/AudioSource.h source/Iron/Audio/AudioListener.cpp
        source/Iron/Audio/AudioListener.h source/Iron/Audio/AudioSystem.cpp source/Iron/Audio/AudioSystem.h
        source/Iron/Audio/WavLoader.cpp source/Iron/Audio/WavLoader.h source/Iron/Physics/PhysicsCore.cpp
        source/Iron/Physics/PhysicsCore.h source/Iron/Physics/Collider.cpp source/Iron/Physics/Collider.h
        source/Iron/Physics/BoxCollider.cpp source/Iron/Physics/BoxCollider.h source/Iron/Physics/RigidBody.cpp
        source/Iron/Physics/RigidBody.h source/Iron/Physics/PhysicsInfo.h source/Iron/Scripting/ScriptingSystem.cpp
        source/Iron/Scripting/ScriptingSystem.h source/Iron/Scripting/InternalCalls.h
        source/Iron/Scripting/InternalCalls.cpp source/Iron/Math/Random.cpp source/Iron/Math/Random.h
        source/Iron/Scripting/ScriptComponent.cpp source/Iron/Scripting/ScriptComponent.h
        source/Iron/Scripting/ScriptingCore.cpp source/Iron/Scripting/ScriptingCore.h
        source/Iron/Scripting/EntityInternalCalls.cpp source/Iron/Scripting/EntityInternalCalls.h
        source/Iron/Scripting/ComponentsInternalCalls.cpp source/Iron/Scripting/ComponentsInternalCalls.h
        source/Iron/Scripting/ScriptingCallsRegister.cpp source/Iron/Scripting/ScriptingCallsRegister.h)

if(MSVC_VERSION GREATER_EQUAL "1700")
    add_compile_options("/MP")
endif()

cmake_policy(SET CMP0072 NEW)

# openal
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    find_package(ALSA REQUIRED)
endif()
add_subdirectory(includes/openal-soft)
set_property(TARGET alrecord PROPERTY FOLDER ${INCLUDE_FOLDER})
set_property(TARGET altonegen PROPERTY FOLDER ${INCLUDE_FOLDER})
set_property(TARGET build_version PROPERTY FOLDER ${INCLUDE_FOLDER})
set_property(TARGET common PROPERTY FOLDER ${INCLUDE_FOLDER})
set_property(TARGET ex-common PROPERTY FOLDER ${INCLUDE_FOLDER})
set_property(TARGET OpenAL PROPERTY FOLDER ${INCLUDE_FOLDER})
set_property(TARGET openal-info PROPERTY FOLDER ${INCLUDE_FOLDER})

# project target
add_library(Iron ${SOURCE_FILES})
set_target_properties(Iron PROPERTIES COMPILE_DEFINITIONS BUILDER_STATIC_DEFINE)

add_custom_command(TARGET Iron COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/includes/dll ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})

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

# openal
target_include_directories(Iron PRIVATE ${ALSOFT_HEADERS_DIR})
target_link_libraries(Iron PRIVATE OpenAL)

# box2d
include_directories(includes/box2d/include)
target_link_libraries(Iron PRIVATE ${PROJECT_SOURCE_DIR}/includes/box2d/cmake-build-debug/bin/box2d.lib)

#mono
include_directories(includes/mono/include)
target_link_libraries(Iron PRIVATE ${PROJECT_SOURCE_DIR}/includes/mono/lib/mono-2.0-sgen.lib)

target_compile_definitions(Iron PRIVATE)
