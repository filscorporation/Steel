cmake_minimum_required(VERSION 3.17)
project(Editor VERSION 0.2)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++ -static-libgcc -pthread -ldl -lpthread -lm")
endif()
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(SOURCE_FILES source/main.cpp source/EditorCore/EditorApplication.cpp source/EditorCore/EditorApplication.h
        source/EditorCore/EditorScene.cpp source/EditorCore/EditorScene.h source/UI/AppView.cpp source/UI/AppView.h
        source/EditorCore/EditorBuilder.cpp source/EditorCore/EditorBuilder.h source/UI/StatsWindow.cpp source/UI/StatsWindow.h
        source/UI/HierarchyView.cpp source/UI/HierarchyView.h source/UI/HierarchyElement.cpp source/UI/HierarchyElement.h
        source/UI/ControlPanel.cpp source/UI/ControlPanel.h
        source/UI/UIEditorTab.cpp source/UI/UIEditorTab.h source/UI/SceneView.cpp source/UI/SceneView.h
        source/EditorCore/EditorSceneRenderer.cpp source/EditorCore/EditorSceneRenderer.h source/EditorCore/EditorTypeSystem.cpp
        source/EditorCore/EditorTypeSystem.h source/UI/PropertiesView.cpp source/UI/PropertiesView.h)

# steel
include_directories(../engine/source)

# project target
add_executable(${CMAKE_PROJECT_NAME} ${SOURCE_FILES})

# glm
include_directories(../engine/includes/glm)

# spdlog
include_directories(../engine/includes/spdlog/include)
add_compile_definitions(SPDLOG_COMPILED_LIB)

# yaml-cpp
include_directories(../engine/includes/yaml-cpp/include)
add_compile_definitions(YAML_CPP_STATIC_DEFINE)

# steel
add_subdirectory(../engine ${CMAKE_LIBRARY_OUTPUT_DIRECTORY} EXCLUDE_FROM_ALL)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE Steel)

# copy precompiled libs to output
add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${PROJECT_SOURCE_DIR}/../engine/includes/dll/
        ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/)
