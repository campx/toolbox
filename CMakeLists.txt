cmake_minimum_required(VERSION 2.8)

project(toolbox LANGUAGES CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -Wextra -Werror")
include_directories(${toolbox_SOURCE_DIR}/include)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(libtoolbox 
    include/toolbox/IteratorTransformer.h   src/IteratorTransformer.cpp
    include/toolbox/ContainerTransformer.h  src/ContainerTransformer.cpp)
set_target_properties(libtoolbox PROPERTIES OUTPUT_NAME toolbox)

add_subdirectory(external/gtest)

target_include_directories(libtoolbox INTERFACE "${toolbox_SOURCE_DIR}/include")

enable_testing()
add_executable(TestToolbox 
               test/ContainerTransformer.cpp
               test/main.cpp)
target_link_libraries(TestToolbox gtest libtoolbox)
set_target_properties(TestToolbox PROPERTIES RUNTIME_OUTPUT_DIRECTORY
                      ${CMAKE_CURRENT_BINARY_DIR}/test)
add_test(NAME TestToolbox
         WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/test
         COMMAND ${CMAKE_CURRENT_BINARY_DIR}/test/TestToolbox)
