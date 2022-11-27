cmake_minimum_required(VERSION 3.17)
project(Application VERSION 0.3)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++ -static-libgcc -pthread -ldl -lpthread -lm")
endif()
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(SOURCE_FILES main.cpp)

# steel
include_directories(../engine/source)

# project target
add_executable(${CMAKE_PROJECT_NAME} ${SOURCE_FILES})

# glm
include_directories(../engine/includes/glm)

# spdlog
include_directories(../engine/includes/spdlog/include)

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
