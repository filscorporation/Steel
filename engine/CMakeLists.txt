cmake_minimum_required(VERSION 3.3)
project(Steel VERSION 0.3)

# detect platform
if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    add_compile_definitions(PLATFORM_LINUX)
    set(TARGET_PLATFORM LINUX)
elseif (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    add_compile_definitions(PLATFORM_WINDOWS)
    set(TARGET_PLATFORM WINDOWS)
elseif (CMAKE_SYSTEM_NAME STREQUAL "Android")
    add_compile_definitions(PLATFORM_ANDROID)
    set(TARGET_PLATFORM ANDROID)
else()
    message("Unknown platform: ${CMAKE_SYSTEM_NAME}")
endif()

# set flags
if (TARGET_PLATFORM MATCHES LINUX)
    set(BUILD_FLAGS "-pthread -ldl -lpthread -lm")
elseif (TARGET_PLATFORM MATCHES WINDOWS)
    set(BUILD_FLAGS "")
elseif (TARGET_PLATFORM MATCHES ANDROID)
    set(BUILD_FLAGS "")
else()
    message("Unknown platform: ${CMAKE_SYSTEM_NAME}")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${BUILD_FLAGS}")
set(CMAKE_CXX_STANDARD 17)

if (${CMAKE_BUILD_TYPE} MATCHES Debug)
    add_compile_definitions(DEBUG_BUILD)
else()
    add_compile_definitions(RELEASE_BUILD)
endif()

# for distribute builds
#add_compile_definitions(DISTRIBUTE_BUILD)

# android is always as distribute
if (TARGET_PLATFORM MATCHES ANDROID)
    add_compile_definitions(DISTRIBUTE_BUILD)
endif()

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# glfw
if (TARGET_PLATFORM MATCHES LINUX OR TARGET_PLATFORM MATCHES WINDOWS)
    set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
endif()

# box2d
set(BOX2D_BUILD_UNIT_TESTS OFF)
set(BOX2D_BUILD_TESTBED OFF)
set(BOX2D_BUILD_EXAMPLES OFF)

# glad
if (TARGET_PLATFORM MATCHES LINUX OR TARGET_PLATFORM MATCHES WINDOWS)
    set(GLAD_FILES includes/glad/src/glad.c)
endif()
set(ZLIB_FILES includes/zlib/zlib.h includes/zlib/adler32.c includes/zlib/compress.c includes/zlib/crc32.c includes/zlib/deflate.c
        includes/zlib/gzclose.c includes/zlib/gzlib.c includes/zlib/gzread.c includes/zlib/gzwrite.c includes/zlib/inflate.c
        includes/zlib/infback.c includes/zlib/inftrees.c includes/zlib/inffast.c includes/zlib/trees.c includes/zlib/uncompr.c includes/zlib/zutil.c)

# android native_app_glue
if (TARGET_PLATFORM MATCHES ANDROID)
    add_library(native_app_glue STATIC ${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c)
endif ()

# platform files
if (TARGET_PLATFORM MATCHES LINUX OR TARGET_PLATFORM MATCHES WINDOWS)
    set(PLATFORM_SOURCE_FILES
            source/Steel/Platform/Rendering/GLADAPI.cpp source/Steel/Platform/Rendering/GLADAPI.h
            source/Steel/Rendering/Core/OpenGLAPI.cpp source/Steel/Rendering/Core/OpenGLAPI.h
            source/Steel/Platform/Window/DesktopWindow.cpp source/Steel/Platform/Window/DesktopWindow.h
            source/Steel/Platform/Time/DesktopTimeProvider.cpp source/Steel/Platform/Time/DesktopTimeProvider.h
            source/Steel/Platform/Input/DesktopInputHandler.cpp source/Steel/Platform/Input/DesktopInputHandler.h
            source/Steel/Platform/Audio/DesktopAudioDevice.h source/Steel/Platform/Audio/DesktopAudioDevice.cpp
            source/Steel/Platform/FilesManager/DesktopFilesManager.cpp source/Steel/Platform/FilesManager/DesktopFilesManager.h)
endif()
if (TARGET_PLATFORM MATCHES ANDROID)
    set(PLATFORM_SOURCE_FILES
            source/Steel/Platform/Application/AndroidApplication.cpp source/Steel/Platform/Application/AndroidApplication.h
            source/Steel/Platform/Rendering/EGLAPI.cpp source/Steel/Platform/Rendering/EGLAPI.h
            source/Steel/Rendering/Core/OpenGLAPI.cpp source/Steel/Rendering/Core/OpenGLAPI.h
            source/Steel/Platform/Window/AndroidWindow.cpp source/Steel/Platform/Window/AndroidWindow.h
            source/Steel/Platform/Time/AndroidTimeProvider.cpp source/Steel/Platform/Time/AndroidTimeProvider.h
            source/Steel/Platform/Input/AndroidInputHandler.cpp source/Steel/Platform/Input/AndroidInputHandler.h
            source/Steel/Platform/Audio/AndroidAudioDevice.cpp source/Steel/Platform/Audio/AndroidAudioDevice.h
            source/Steel/Platform/FilesManager/AndroidFilesManager.cpp source/Steel/Platform/FilesManager/AndroidFilesManager.h)
endif()

set(INCLUDE_FOLDER "includes")
set(SOURCE_FILES
        ${GLAD_FILES} ${ZLIB_FILES} ${PLATFORM_SOURCE_FILES}
        source/Steel/Rendering/MaterialSystem/Shader.h source/Steel/Rendering/MaterialSystem/Shader.cpp source/Steel/Core/Application.cpp
        source/Steel/Core/Application.h source/Steel/Input/Input.cpp source/Steel/Input/Input.h source/Steel/Input/KeyCodes.h
        source/Steel/Input/MouseCodes.h source/Steel.h source/Steel/EntityComponentSystem/Component.cpp
        source/Steel/EntityComponentSystem/Component.h source/Steel/Rendering/SpriteRenderer.cpp
        source/Steel/Rendering/SpriteRenderer.h source/Steel/Scene/Scene.cpp source/Steel/Scene/Scene.h
        source/Steel/Scene/Transformation.cpp source/Steel/Scene/Transformation.h source/Steel/Core/Log.cpp
        source/Steel/Core/Log.h source/Steel/Rendering/Camera.cpp source/Steel/Rendering/Camera.h source/Steel/Core/Time.cpp
        source/Steel/Core/Time.h source/Steel/Rendering/Screen.cpp source/Steel/Rendering/Screen.h
        source/Steel/Resources/ResourcesManager.cpp source/Steel/Resources/ResourcesManager.h source/Steel/Rendering/Renderer.cpp
        source/Steel/Rendering/Renderer.h source/Steel/Animation/Animator.cpp source/Steel/Animation/Animator.h
        source/Steel/Animation/Animation.cpp source/Steel/Animation/Animation.h source/Steel/Rendering/Sprite.cpp
        source/Steel/Rendering/Sprite.h source/Steel/Audio/AudioTrack.cpp source/Steel/Audio/AudioTrack.h
        source/Steel/Audio/AudioSource.cpp source/Steel/Audio/AudioSource.h source/Steel/Audio/AudioListener.cpp
        source/Steel/Audio/AudioListener.h source/Steel/Audio/AudioCore.cpp source/Steel/Audio/AudioCore.h
        source/Steel/Audio/WavLoader.cpp source/Steel/Audio/WavLoader.h source/Steel/Physics/PhysicsCore.cpp
        source/Steel/Physics/PhysicsCore.h source/Steel/Physics/Collider.cpp source/Steel/Physics/Collider.h
        source/Steel/Physics/BoxCollider.cpp source/Steel/Physics/BoxCollider.h source/Steel/Physics/RigidBody.cpp
        source/Steel/Physics/RigidBody.h source/Steel/Physics/PhysicsInfo.h source/Steel/Scripting/ScriptingSystem.cpp
        source/Steel/Scripting/ScriptingSystem.h source/Steel/Scripting/InternalCalls/CoreInternalCalls.h
        source/Steel/Scripting/InternalCalls/CoreInternalCalls.cpp source/Steel/Math/Random.cpp source/Steel/Math/Random.h
        source/Steel/Scripting/ScriptComponent.cpp source/Steel/Scripting/ScriptComponent.h
        source/Steel/Scripting/ScriptingCore.cpp source/Steel/Scripting/ScriptingCore.h
        source/Steel/Scripting/InternalCalls/EntityInternalCalls.cpp source/Steel/Scripting/InternalCalls/EntityInternalCalls.h
        source/Steel/Scripting/InternalCalls/ComponentsInternalCalls.cpp source/Steel/Scripting/InternalCalls/ComponentsInternalCalls.h
        source/Steel/Scripting/ScriptingCallsRegister.cpp source/Steel/Scripting/ScriptingCallsRegister.h
        source/Steel/Rendering/MaterialSystem/BuiltInShaders.h source/Steel/Physics/Physics.cpp source/Steel/Physics/Physics.h
        source/Steel/Physics/PhysicsContactListener.cpp source/Steel/Physics/PhysicsContactListener.h
        source/Steel/Physics/Collision.h source/Steel/UI/RectTransformation.cpp source/Steel/UI/RectTransformation.h
        source/Steel/UI/UIComponent.cpp source/Steel/UI/UIComponent.h
        source/Steel/UI/UIElements/UIImage.cpp source/Steel/UI/UIElements/UIImage.h
        source/Steel/Scripting/InternalCalls/UIInternalCalls.cpp source/Steel/Scripting/InternalCalls/UIInternalCalls.h
        source/Steel/EntityComponentSystem/EntitiesRegistry.h source/Steel/EntityComponentSystem/Entity.h
        source/Steel/Scene/NameComponent.cpp source/Steel/Scene/NameComponent.h source/Steel/Scene/SceneHelper.h
        source/Steel/Scene/SceneHelper.h source/Steel/Scripting/InternalCalls/InternalCallsCommon.h source/Steel/Resources/ResourceID.h
        source/Steel/EntityComponentSystem/SparseDataSet.h
        source/Steel/Physics/CircleCollider.cpp source/Steel/Physics/CircleCollider.h
        source/Steel/Scene/HierarchyNode.cpp source/Steel/Scene/HierarchyNode.h source/Steel/Scene/Hierarchy.h
        source/Steel/Scene/Hierarchy.cpp source/Steel/UI/UILayer.cpp source/Steel/UI/UILayer.h
        source/Steel/UI/UIEventHandler.cpp source/Steel/UI/UIEventHandler.h source/Steel/UI/UIEvent.h source/Steel/Input/ButtonStates.h
        source/Steel/UI/UIElements/UIButton.cpp source/Steel/UI/UIElements/UIButton.h source/Steel/UI/UIElements/UIText.cpp source/Steel/UI/UIElements/UIText.h
        source/Steel/UI/Font.cpp source/Steel/UI/Font.h
        source/Steel/UI/FontManager.cpp source/Steel/UI/FontManager.h
        source/Steel/EntityComponentSystem/SparseSet.h source/Steel/Math/Math.h
        source/Steel/Math/PerlinNoise.cpp source/Steel/Math/PerlinNoise.h
        source/Steel/Resources/PngLoader.cpp source/Steel/Resources/PngLoader.h
        source/Steel/Resources/AsepriteLoader.cpp source/Steel/Resources/AsepriteLoader.h source/Steel/Resources/AsepriteData.h
        source/Steel/Resources/FileLoader.cpp source/Steel/Resources/FileLoader.h
        source/Steel/Scripting/ScriptingCommon.h source/Steel/UI/UIElements/UIInputField.cpp
        source/Steel/UI/UIElements/UIInputField.h source/Steel/UI/TransitionInfo.h source/Steel/UI/UIInteractable.cpp source/Steel/UI/UIInteractable.h
        source/Steel/UI/CallbackType.h source/Steel/Scene/HierarchyParent.h source/Steel/Rendering/MaterialSystem/Material.cpp source/Steel/Rendering/MaterialSystem/Material.h
        source/Steel/Rendering/MaterialSystem/MaterialPropertyBlock.cpp source/Steel/Rendering/MaterialSystem/MaterialPropertyBlock.h source/Steel/Scripting/InternalStructures/MaterialPropertyBlockInternal.h
        source/Steel/Scripting/InternalStructures/MaterialPropertyBlockInternal.cpp source/Steel/UI/UIElements/UIClipping.cpp source/Steel/UI/UIElements/UIClipping.h
        source/Steel/UI/UIElements/UICheckBox.cpp source/Steel/UI/UIElements/UICheckBox.h
        source/Steel/UI/UIElements/UITabs.cpp source/Steel/UI/UIElements/UITabs.h source/Steel/UI/UIElements/UILayoutGroup.cpp
        source/Steel/UI/UIElements/UILayoutGroup.h source/Steel/UI/UIElements/LayoutGroupTypes.h
        source/Steel/Scene/SceneManager.cpp source/Steel/Scene/SceneManager.h source/Steel/Rendering/Framebuffer.cpp source/Steel/Rendering/Framebuffer.h
        source/Steel/Rendering/MaterialSystem/Texture.cpp source/Steel/Rendering/MaterialSystem/Texture.h
        source/Steel/Rendering/SceneRenderer.cpp source/Steel/Rendering/SceneRenderer.h source/Steel/Serialization/SerializationManager.cpp
        source/Steel/Serialization/SerializationManager.h source/Steel/Serialization/Serializable.h source/Steel/Serialization/AttributeInfo.h
        source/Steel/Serialization/AttributeAccessorBase.h source/Steel/Common/StringUtils.h source/Steel/Serialization/SerializationConvert.h
        source/Steel/EntityComponentSystem/TypeInfo.h source/Steel/EntityComponentSystem/TypeInfo.cpp source/Steel/EntityComponentSystem/TypeInfoStorage.cpp source/Steel/EntityComponentSystem/TypeInfoStorage.h
        source/Steel/Scene/HierarchyParent.cpp source/Steel/Rendering/Core/DrawCall.h source/Steel/Rendering/Core/RenderList.cpp
        source/Steel/Rendering/Core/RenderList.h source/Steel/Rendering/Core/RenderContext.h source/Steel/Rendering/Core/VertexBuffer.cpp
        source/Steel/Rendering/Core/VertexBuffer.h source/Steel/Rendering/Core/RenderTask.h source/Steel/Rendering/Core/DrawCallsSource.h
        source/Steel/Rendering/SceneDrawCallsSource.cpp source/Steel/Rendering/SceneDrawCallsSource.h source/Steel/Rendering/UIDrawCallsSource.cpp
        source/Steel/Rendering/UIDrawCallsSource.h source/Steel/Common/FlagType.h source/Steel/Rendering/Core/IndexBuffer.cpp source/Steel/Rendering/Core/IndexBuffer.h
        source/Steel/Rendering/Core/RendererEnums.h source/Steel/Rendering/Core/VertexAttribute.h source/Steel/Math/Sorting.h
        source/Steel/Rendering/MeshRenderer.cpp source/Steel/Rendering/MeshRenderer.h source/Steel/Rendering/Mesh.cpp source/Steel/Rendering/Mesh.h
        source/Steel/Scripting/InternalStructures/MeshInternal.cpp source/Steel/Scripting/InternalStructures/MeshInternal.h source/Steel/Scene/IDComponent.cpp
        source/Steel/Scene/IDComponent.h source/Steel/Serialization/SerializationContext.h source/Steel/Serialization/AttributeAccessor.h
        source/Steel/Serialization/IDAttributeAccessor.h source/Steel/Scene/UUID.h source/Steel/Serialization/SerializationContext.cpp source/Steel/Core/Components.h
        source/Steel/Resources/ResourceType.h source/Steel/Resources/Resource.h source/Steel/Resources/AsepriteData.cpp
        source/Steel/Serialization/ResourceAttributeAccessor.h source/Steel/Serialization/EnumAttributeAccessor.h source/Steel/EntityComponentSystem/CoreTypeSystem.cpp
        source/Steel/EntityComponentSystem/CoreTypeSystem.h source/Steel/Scripting/SimpleAPITypes.h source/Steel/EntityComponentSystem/ComponentsPoolWrapper.h
        source/Steel/EntityComponentSystem/ComponentIterator.h source/Steel/EntityComponentSystem/ComponentAccessor.h source/Steel/EntityComponentSystem/Entity.cpp
        source/Steel/EntityComponentSystem/EntitiesIterator.h source/Steel/Serialization/ListAttributeAccessor.h source/Steel/UI/UIElements/UITabInfo.h
        source/Steel/UI/UIElements/UITabInfo.cpp source/Steel/UI/UIElements/UILayoutElementInfo.cpp source/Steel/UI/UIElements/UILayoutElementInfo.h
        source/Steel/EntityComponentSystem/RawComponentData.h source/Steel/Serialization/ScriptsListAttributeAccessor.h
        source/Steel/EntityComponentSystem/Types.h source/Steel/Scripting/Accessors/ScriptAttributeAccessor.h source/Steel/Scripting/Accessors/ScriptAttributeAccessorBase.h
        source/Steel/Scripting/ScriptObjectHandler.h source/Steel/Scripting/ScriptObjectHandler.cpp source/Steel/Scripting/ScriptMethodPointer.h
        source/Steel/Scripting/APIStructs.h source/Steel/Scripting/ScriptingDomain.h source/Steel/Scripting/ScriptSerializationHelper.cpp
        source/Steel/Scripting/ScriptSerializationHelper.h source/Steel/Scripting/Accessors/ScriptStringAttributeAccessor.h
        source/Steel/Resources/SceneData.cpp source/Steel/Resources/SceneData.h source/Steel/Scene/SceneBackup.h source/Steel/Scripting/InternalCalls/ResourcesInternalCalls.cpp
        source/Steel/Scripting/InternalCalls/ResourcesInternalCalls.h source/Steel/UI/UIElements/UIScrollableView.cpp source/Steel/UI/UIElements/UIScrollableView.h
        source/Steel/Serialization/AttributesRegistration.h source/Steel/Core/Resources.h source/Steel/Core/ApplicationConfig.h source/Steel/Rendering/Window.h
        source/Steel/Core/TimeProvider.h source/Steel/Input/InputHandler.h source/Steel/Input/InputHandler.cpp source/Steel/Audio/AudioDevice.h
        source/Steel/Resources/FilesManager.h source/Steel/Resources/FileData.h source/Steel/Resources/FileDataReader.cpp source/Steel/Resources/FileDataReader.h
        source/Steel/Resources/ShaderLoader.cpp source/Steel/Resources/ShaderLoader.h)

if(MSVC_VERSION GREATER_EQUAL "1700")
    add_compile_options("/MP")
endif()

cmake_policy(SET CMP0048 NEW)
cmake_policy(SET CMP0072 NEW)
set(CMAKE_POLICY_DEFAULT_CMP0048 NEW)
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)

# openal
if (TARGET_PLATFORM MATCHES LINUX OR TARGET_PLATFORM MATCHES WINDOWS)
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
endif ()

# project target
add_library(Steel ${SOURCE_FILES})
set_target_properties(Steel PROPERTIES COMPILE_DEFINITIONS BUILDER_STATIC_DEFINE)

# copy precompiled libs to output
add_custom_command(TARGET Steel POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${PROJECT_SOURCE_DIR}/includes/dll/
        ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/)

include_directories(source)

# opengl
if (TARGET_PLATFORM MATCHES LINUX OR TARGET_PLATFORM MATCHES WINDOWS)
    find_package(OpenGL REQUIRED)
    target_link_libraries(Steel PRIVATE ${OPENGL_LIBRARIES})
endif ()
if (TARGET_PLATFORM MATCHES ANDROID)
    target_link_libraries(Steel PRIVATE android EGL GLESv3)
endif ()

# stb
include_directories(includes)

# glad
if (TARGET_PLATFORM MATCHES LINUX OR TARGET_PLATFORM MATCHES WINDOWS)
    include_directories(includes/glad/include)
endif ()

# android native_app_glue
if (TARGET_PLATFORM MATCHES ANDROID)
    target_include_directories(Steel PRIVATE ${ANDROID_NDK}/sources/android/native_app_glue)
    target_link_libraries(Steel PRIVATE android native_app_glue EGL GLESv1_CM log)
endif ()

# zlib
include_directories(includes/zlib)

# glfw
if (TARGET_PLATFORM MATCHES LINUX OR TARGET_PLATFORM MATCHES WINDOWS)
    add_subdirectory(includes/glfw EXCLUDE_FROM_ALL)
    target_link_libraries(Steel PRIVATE glfw)
endif ()

# glm
add_subdirectory(includes/glm EXCLUDE_FROM_ALL)
include_directories(includes/glm)
target_link_libraries(Steel PRIVATE glm)

# openal
if (TARGET_PLATFORM MATCHES LINUX OR TARGET_PLATFORM MATCHES WINDOWS)
    target_include_directories(Steel PRIVATE ${ALSOFT_HEADERS_DIR})
    target_link_libraries(Steel PRIVATE OpenAL)
endif ()

# box2d
add_subdirectory(includes/box2d EXCLUDE_FROM_ALL)
target_link_libraries(Steel PRIVATE box2d)

# mono
if (TARGET_PLATFORM MATCHES LINUX)
    set(MONO_NAME "/usr/lib/libmonosgen-2.0.so")
elseif (TARGET_PLATFORM MATCHES WINDOWS)
    set(MONO_NAME "${PROJECT_SOURCE_DIR}/includes/mono/lib/mono-2.0-sgen.lib")
elseif (TARGET_PLATFORM MATCHES ANDROID)
    set(MONO_NAME "${PROJECT_SOURCE_DIR}/includes/mono/lib/libmonobdwgc-2.0.so")
endif ()

# mono
include_directories(includes/mono/include)
target_link_libraries(Steel PRIVATE ${MONO_NAME})

# freetype
add_subdirectory(includes/freetype2 EXCLUDE_FROM_ALL)
include_directories(includes/freetype2/include)
target_link_libraries(Steel PRIVATE freetype)

# spdlog
add_subdirectory(includes/spdlog EXCLUDE_FROM_ALL)
include_directories(includes/spdlog/include)
target_link_libraries(Steel PRIVATE spdlog)

# yaml cpp
add_subdirectory(includes/yaml-cpp EXCLUDE_FROM_ALL)
include_directories(includes/yaml-cpp/include)
target_link_libraries(Steel PRIVATE yaml-cpp)

target_compile_definitions(Steel PRIVATE)
