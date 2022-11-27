cmake_minimum_required(VERSION 3.17)
project(AutoTests VERSION 0.2)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++ -static-libgcc -pthread -ldl -lpthread -lm")
endif()
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# google test
add_subdirectory(includes/googletest)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

set(TESTS_PROJECT_SOURCES main.cpp TestsSetUp.h TestsSetUp.cpp)
set(TESTS_FILES ApplicationTest.cpp SparseSetTest.cpp SparseDataSetTest.cpp EntitiesRegistryTest.cpp common/TestComponent1.cpp
        common/TestComponent1.h common/TestComponent2.cpp common/TestComponent2.h HierarchyTest.cpp ActivationTest.cpp
        SerializationTest.cpp common/SerializedComponent.cpp common/SerializedComponent.h common/TestType.cpp common/TestType.h)

# target
add_executable(RunAutoTests ${TESTS_PROJECT_SOURCES} ${TESTS_FILES})
target_link_libraries(RunAutoTests gtest gtest_main)

# steel
include_directories(../engine/source)

# glm
include_directories(../engine/includes/glm)

# spdlog
include_directories(../engine/includes/spdlog/include)
add_compile_definitions(SPDLOG_COMPILED_LIB)

# yaml cpp
include_directories(../engine/includes/yaml-cpp/include)
add_compile_definitions(YAML_CPP_STATIC_DEFINE)

# steel
add_subdirectory(../engine ${CMAKE_LIBRARY_OUTPUT_DIRECTORY} EXCLUDE_FROM_ALL)
target_link_libraries(RunAutoTests Steel)
