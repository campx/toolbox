cmake_minimum_required(VERSION 2.8)
project(toolbox LANGUAGES CXX)

if (NOT ${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wextra")
endif()

SET(GTEST_LIB gtest)
if (${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
    if (${CMAKE_BUILD_TYPE} STREQUAL Debug)
        SET(GTEST_LIB gtestd)
    endif()
endif()

include_directories(${CMAKE_BINARY_DIR}/include)
link_directories(${CMAKE_BINARY_DIR}/lib)

add_subdirectory(external/gtest)

add_library(libtoolbox 
    include/toolbox/IteratorTransformer.h   src/IteratorTransformer.cpp
    include/toolbox/IteratorRecorder.h      src/IteratorRecorder.cpp
    include/toolbox/Iterator.h              src/Iterator.cpp
    include/toolbox/ContainerTransformer.h  src/ContainerTransformer.cpp
    include/toolbox/SequencePredicate.h     src/SequencePredicate.cpp
    include/toolbox/LazyEvaluation.h        src/LazyEvaluation.cpp
    include/toolbox/Value.h                 src/Value.cpp)

set_target_properties(libtoolbox PROPERTIES OUTPUT_NAME toolbox)
target_compile_features(libtoolbox PUBLIC cxx_return_type_deduction)
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

set_target_properties(TestToolbox PROPERTIES RUNTIME_OUTPUT_DIRECTORY
                      ${CMAKE_CURRENT_BINARY_DIR}/test)

target_link_libraries(TestToolbox ${GTEST_LIB} libtoolbox)

add_dependencies(TestToolbox googletest)
add_test(NAME TestToolbox
         WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/test
         COMMAND ${CMAKE_CURRENT_BINARY_DIR}/test/TestToolbox)
