cmake_minimum_required(VERSION 3.17)
project(Application VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 11)
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++ -static-libgcc")
endif()

set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(SOURCE_FILES main.cpp)

# iron
include_directories(../engine/source)

# project target
add_executable(${CMAKE_PROJECT_NAME} ${SOURCE_FILES})

# iron
add_subdirectory(../engine ${CMAKE_BINARY_DIR}/bin EXCLUDE_FROM_ALL)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE Iron)

# glm
if (NOT TARGET glm)
    add_subdirectory(includes/glm EXCLUDE_FROM_ALL)
    target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE glm)
else()
    include_directories(../engine/includes/glm)
endif()
