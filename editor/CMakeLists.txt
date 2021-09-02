cmake_minimum_required(VERSION 3.17)
project(Editor VERSION 0.2)

set(CMAKE_CXX_STANDARD 11)
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++ -static-libgcc -pthread -ldl -lpthread -lm")
endif()

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(SOURCE_FILES main.cpp)

# iron
include_directories(../engine/source)

# project target
add_executable(${CMAKE_PROJECT_NAME} ${SOURCE_FILES})

# glm
include_directories(../engine/includes/glm)

# spdlog
include_directories(../engine/includes/spdlog/include)

# iron
add_subdirectory(../engine ${CMAKE_LIBRARY_OUTPUT_DIRECTORY} EXCLUDE_FROM_ALL)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE Iron)

# copy precompiled libs to output
add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${PROJECT_SOURCE_DIR}/../engine/includes/dll/
        ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/)
