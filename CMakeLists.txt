cmake_minimum_required(VERSION 2.8)

project(toolbox LANGUAGES CXX)

if (${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
    message(STATUS "Using MSVC")
else()
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wextra")
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang") # using Clang
        message(STATUS "Using Clang")
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
    elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL GNU) # using GCC
        message(STATUS "Using GNU")
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++14")
    endif()
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(libtoolbox 
    include/toolbox/IteratorTransformer.h   src/IteratorTransformer.cpp
    include/toolbox/IteratorRecorder.h      src/IteratorRecorder.cpp
    include/toolbox/Iterator.h              src/Iterator.cpp
    include/toolbox/ContainerTransformer.h  src/ContainerTransformer.cpp
    include/toolbox/SequencePredicate.h     src/SequencePredicate.cpp
    include/toolbox/LazyEvaluation.h        src/LazyEvaluation.cpp
    include/toolbox/Value.h                 src/Value.cpp)

set_target_properties(libtoolbox PROPERTIES OUTPUT_NAME toolbox)

add_subdirectory(external/gtest)

target_include_directories(libtoolbox PUBLIC "${toolbox_SOURCE_DIR}/include")

enable_testing()
add_executable(TestToolbox 
               test/ContainerTransformer.cpp
               test/Value.cpp
               test/SequencePredicate.cpp
               test/IteratorRecorder.cpp
               test/Iterator.cpp
               test/LazyEvaluation.cpp
               test/main.cpp)
target_link_libraries(TestToolbox gtest libtoolbox)
set_target_properties(TestToolbox PROPERTIES RUNTIME_OUTPUT_DIRECTORY
                      ${CMAKE_CURRENT_BINARY_DIR}/test)
add_test(NAME TestToolbox
         WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/test
         COMMAND ${CMAKE_CURRENT_BINARY_DIR}/test/TestToolbox)
