cmake_minimum_required(VERSION 2.8)

project(aux LANGUAGES CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -Wextra -Werror")
include_directories(${aux_SOURCE_DIR}/include)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(libaux src/IteratorAdapter.cpp src/MapAdapter.cpp src/HashMap.cpp)
set_target_properties(libaux PROPERTIES OUTPUT_NAME aux)

add_subdirectory(external/gtest)

target_include_directories(libaux INTERFACE "${aux_SOURCE_DIR}/include")

enable_testing()
add_executable(TestAux test/TestMapAdapter.cpp test/TestHashMap.cpp
               test/main.cpp)
target_link_libraries(TestAux gtest libaux)
set_target_properties(TestAux PROPERTIES RUNTIME_OUTPUT_DIRECTORY
                      ${CMAKE_CURRENT_BINARY_DIR}/test)
add_test(NAME TestAux
         WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/test
         COMMAND ${CMAKE_CURRENT_BINARY_DIR}/test/TestAux)
