cmake_minimum_required(VERSION 3.17)
project(Editor VERSION 0.2)

set(CMAKE_CXX_STANDARD 11)
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP -static-libstdc++ -static-libgcc -pthread -ldl -lpthread -lm")
endif()

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(SOURCE_FILES source/main.cpp source/EditorCore/EditorApplication.cpp source/EditorCore/EditorApplication.h
        source/EditorCore/EditorScene.cpp source/EditorCore/EditorScene.h source/UI/AppView.cpp source/UI/AppView.h
        source/EditorCore/EditorBuilder.cpp source/EditorCore/EditorBuilder.h source/UI/StatsWindow.cpp source/UI/StatsWindow.h)

# steel
include_directories(../engine/source)

# project target
add_executable(${CMAKE_PROJECT_NAME} ${SOURCE_FILES})

# glm
include_directories(../engine/includes/glm)

# spdlog
include_directories(../engine/includes/spdlog/include)

# steel
add_subdirectory(../engine ${CMAKE_LIBRARY_OUTPUT_DIRECTORY} EXCLUDE_FROM_ALL)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE Steel)

# copy precompiled libs to output
add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${PROJECT_SOURCE_DIR}/../engine/includes/dll/
        ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/)
