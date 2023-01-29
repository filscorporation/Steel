cmake_minimum_required(VERSION 3.17)
project(SteelAndroid VERSION 0.3)

# set flags
set(BUILD_FLAGS "")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${BUILD_FLAGS}")
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(SOURCE_FILES main.cpp)
set(ENGINE_PATH ../../../../../../engine)

# steel
include_directories(${ENGINE_PATH}/source)

# project target
add_library(${CMAKE_PROJECT_NAME} SHARED ${SOURCE_FILES})

# glm
include_directories(${ENGINE_PATH}/includes/glm)

# spdlog
include_directories(${ENGINE_PATH}/includes/spdlog/include)

# yaml-cpp
include_directories(${ENGINE_PATH}/includes/yaml-cpp/include)
add_compile_definitions(YAML_CPP_STATIC_DEFINE)

# steel
add_subdirectory(${ENGINE_PATH} ${CMAKE_LIBRARY_OUTPUT_DIRECTORY} EXCLUDE_FROM_ALL)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE Steel)

# copy precompiled libs to output
add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${PROJECT_SOURCE_DIR}/${ENGINE_PATH}/includes/dll/
        ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/)
