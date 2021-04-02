cmake_minimum_required(VERSION 3.3)
project(Iron VERSION 0.0.1)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread -ldl -lpthread -lm")
endif()

if (${CMAKE_BUILD_TYPE} MATCHES Debug)
    add_compile_definitions(DEBUG_BUILD)
else()
    add_compile_definitions(RELEASE_BUILD)
endif()

# For distribute builds
#add_compile_definitions(DISTRIBUTE_BUILD)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

set(BOX2D_BUILD_UNIT_TESTS OFF)
set(BOX2D_BUILD_TESTBED OFF)
set(BOX2D_BUILD_EXAMPLES OFF)

set(GLAD_FILES includes/glad/src/glad.c)
set(INCLUDE_FOLDER "includes")
set(SOURCE_FILES
        ${GLAD_FILES}
        source/Iron/Rendering/Shader.h source/Iron/Rendering/Shader.cpp source/Iron/Core/Application.cpp
        source/Iron/Core/Application.h source/Iron/Core/Input.cpp source/Iron/Core/Input.h source/Iron/Core/KeyCodes.h
        source/Iron/Core/MouseCodes.h source/Iron.h source/Iron/EntityComponentSystem/Component.cpp
        source/Iron/EntityComponentSystem/Component.h source/Iron/Rendering/SpriteRenderer.cpp
        source/Iron/Rendering/SpriteRenderer.h source/Iron/Scene/Scene.cpp source/Iron/Scene/Scene.h
        source/Iron/Scene/Transformation.cpp source/Iron/Scene/Transformation.h source/Iron/Core/Log.cpp
        source/Iron/Core/Log.h source/Iron/Rendering/Camera.cpp source/Iron/Rendering/Camera.h source/Iron/Core/Time.cpp
        source/Iron/Core/Time.h source/Iron/Rendering/Screen.cpp source/Iron/Rendering/Screen.h
        source/Iron/Core/ResourcesManager.cpp source/Iron/Core/ResourcesManager.h source/Iron/Rendering/Renderer.cpp
        source/Iron/Rendering/Renderer.h source/Iron/Animation/Animator.cpp source/Iron/Animation/Animator.h
        source/Iron/Animation/Animation.cpp source/Iron/Animation/Animation.h source/Iron/Rendering/Sprite.cpp
        source/Iron/Rendering/Sprite.h source/Iron/Audio/AudioTrack.cpp source/Iron/Audio/AudioTrack.h
        source/Iron/Audio/AudioSource.cpp source/Iron/Audio/AudioSource.h source/Iron/Audio/AudioListener.cpp
        source/Iron/Audio/AudioListener.h source/Iron/Audio/AudioCore.cpp source/Iron/Audio/AudioCore.h
        source/Iron/Audio/WavLoader.cpp source/Iron/Audio/WavLoader.h source/Iron/Physics/PhysicsCore.cpp
        source/Iron/Physics/PhysicsCore.h source/Iron/Physics/Collider.cpp source/Iron/Physics/Collider.h
        source/Iron/Physics/BoxCollider.cpp source/Iron/Physics/BoxCollider.h source/Iron/Physics/RigidBody.cpp
        source/Iron/Physics/RigidBody.h source/Iron/Physics/PhysicsInfo.h source/Iron/Scripting/ScriptingSystem.cpp
        source/Iron/Scripting/ScriptingSystem.h source/Iron/Scripting/CoreInternalCalls.h
        source/Iron/Scripting/CoreInternalCalls.cpp source/Iron/Math/Random.cpp source/Iron/Math/Random.h
        source/Iron/Scripting/ScriptComponent.cpp source/Iron/Scripting/ScriptComponent.h
        source/Iron/Scripting/ScriptingCore.cpp source/Iron/Scripting/ScriptingCore.h
        source/Iron/Scripting/EntityInternalCalls.cpp source/Iron/Scripting/EntityInternalCalls.h
        source/Iron/Scripting/ComponentsInternalCalls.cpp source/Iron/Scripting/ComponentsInternalCalls.h
        source/Iron/Scripting/ScriptingCallsRegister.cpp source/Iron/Scripting/ScriptingCallsRegister.h
        source/Iron/Rendering/BuiltInShaders.h source/Iron/Physics/Physics.cpp source/Iron/Physics/Physics.h
        source/Iron/Physics/PhysicsContactListener.cpp source/Iron/Physics/PhysicsContactListener.h
        source/Iron/Physics/Collision.h source/Iron/UI/RectTransformation.cpp source/Iron/UI/RectTransformation.h
        source/Iron/UI/UIComponent.cpp source/Iron/UI/UIComponent.h source/Iron/UI/UIRenderer.cpp
        source/Iron/UI/UIRenderer.h source/Iron/UI/UIElements/UIImage.cpp source/Iron/UI/UIElements/UIImage.h
        source/Iron/Scripting/UIInternalCalls.cpp source/Iron/Scripting/UIInternalCalls.h
        source/Iron/EntityComponentSystem/EntitiesRegistry.h source/Iron/EntityComponentSystem/Entity.h
        source/Iron/Scene/NameComponent.cpp source/Iron/Scene/NameComponent.h source/Iron/Scene/SceneHelper.h
        source/Iron/Scene/SceneHelper.h source/Iron/Scripting/InternalCallsCommon.h source/Iron/Core/ResourceID.h
        source/Iron/EntityComponentSystem/SparseDataSet.h source/Iron/EntityComponentSystem/ComponentSystem.h
        source/Iron/Physics/PhysicsSystem.cpp source/Iron/Physics/PhysicsSystem.h source/Iron/Audio/AudioSystem.cpp
        source/Iron/Audio/AudioSystem.h source/Iron/Physics/CircleCollider.cpp source/Iron/Physics/CircleCollider.h
        source/Iron/Scene/HierarchyNode.cpp source/Iron/Scene/HierarchyNode.h source/Iron/Scene/Hierarchy.h
        source/Iron/Scene/Hierarchy.cpp source/Iron/Rendering/RendererData.h source/Iron/UI/UILayer.cpp source/Iron/UI/UILayer.h
        source/Iron/UI/UIEventHandler.cpp source/Iron/UI/UIEventHandler.h source/Iron/UI/UIEvent.h source/Iron/Core/ButtonStates.h
        source/Iron/UI/UIElements/UIButton.cpp source/Iron/UI/UIElements/UIButton.h source/Iron/UI/UIElements/UIText.cpp source/Iron/UI/UIElements/UIText.h
        source/Iron/UI/UIElements/UITextLetter.cpp source/Iron/UI/UIElements/UITextLetter.h source/Iron/UI/Font.cpp source/Iron/UI/Font.h
        source/Iron/UI/FontManager.cpp source/Iron/UI/FontManager.h source/Iron/UI/UILetterRenderer.cpp
        source/Iron/UI/UILetterRenderer.h source/Iron/EntityComponentSystem/SparseSet.h source/Iron/Math/Math.h
        source/Iron/Math/PerlinNoise.cpp source/Iron/Math/PerlinNoise.h source/Iron/Scene/HierarchySystem.cpp
        source/Iron/Scene/HierarchySystem.h source/Iron/Scene/TransformationSystem.cpp source/Iron/Scene/TransformationSystem.h
        source/Iron/UI/UISystem.cpp source/Iron/UI/UISystem.h source/Iron/Scripting/ScriptComponentSystem.cpp source/Iron/Scripting/ScriptComponentSystem.h
        source/Iron/Debug/Debug.cpp source/Iron/Debug/Debug.h source/Iron/Debug/DebugInfoWindow.cpp source/Iron/Debug/DebugInfoWindow.h)

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

# copy precompiled libs to output
add_custom_command(TARGET Iron POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${PROJECT_SOURCE_DIR}/includes/dll/
        ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/)

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
include_directories(includes/glm)
target_link_libraries(Iron PRIVATE glm)

# openal
target_include_directories(Iron PRIVATE ${ALSOFT_HEADERS_DIR})
target_link_libraries(Iron PRIVATE OpenAL)

if (UNIX AND NOT APPLE)
    set(MONO_NAME "/usr/lib/libmonosgen-2.0.so")
    set(FREETYPE_NAME "freetype.a")
endif ()
if (WIN32)
    set(MONO_NAME "${PROJECT_SOURCE_DIR}/includes/mono/lib/mono-2.0-sgen.lib")
    set(FREETYPE_NAME "freetype.lib")
endif ()

# box2d
add_subdirectory(includes/box2d EXCLUDE_FROM_ALL)
target_link_libraries(Iron PRIVATE box2d)

#mono
include_directories(includes/mono/include)
target_link_libraries(Iron PRIVATE ${MONO_NAME})

#freetype
add_subdirectory(includes/freetype2 EXCLUDE_FROM_ALL)
include_directories(includes/freetype2/include)
target_link_libraries(Iron PRIVATE freetype)

target_compile_definitions(Iron PRIVATE)
