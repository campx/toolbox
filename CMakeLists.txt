cmake_minimum_required(VERSION 2.8)
project(toolbox LANGUAGES CXX)

if (NOT ${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wextra")
endif()

SET(GTEST_LIBRARIES gtest)
if (${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
    if (${CMAKE_BUILD_TYPE} STREQUAL Debug)
        SET(GTEST_LIBRARIES gtestd)
    endif()
endif()

include_directories(${CMAKE_BINARY_DIR}/include)
link_directories(${CMAKE_BINARY_DIR}/lib)

add_subdirectory(external/gtest)

add_library(libtoolbox 
    toolbox/IteratorTransformer.h   toolbox/IteratorTransformer.cpp
    toolbox/IteratorRecorder.h      toolbox/IteratorRecorder.cpp
    toolbox/Iterator.h              toolbox/Iterator.cpp
    toolbox/ContainerTransformer.h  toolbox/ContainerTransformer.cpp
    toolbox/SequencePredicate.h     toolbox/SequencePredicate.cpp
    toolbox/LazyEvaluation.h        toolbox/LazyEvaluation.cpp
    toolbox/Value.h                 toolbox/Value.cpp)

set_target_properties(libtoolbox PROPERTIES OUTPUT_NAME toolbox)
target_compile_features(libtoolbox PUBLIC cxx_return_type_deduction)
target_include_directories(libtoolbox PUBLIC "${toolbox_SOURCE_DIR}")

install(TARGETS libtoolbox 
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin)
install(DIRECTORY toolbox DESTINATION include FILES_MATCHING 
        PATTERN "*.h")

enable_testing()
add_executable(TestToolbox 
               toolbox/test/ContainerTransformer.cpp
               toolbox/test/Value.cpp
               toolbox/test/SequencePredicate.cpp
               toolbox/test/IteratorRecorder.cpp
			   toolbox/test/IteratorTransformer.cpp
               toolbox/test/Iterator.cpp
               toolbox/test/LazyEvaluation.cpp
               toolbox/test/main.cpp)

set_target_properties(TestToolbox PROPERTIES RUNTIME_OUTPUT_DIRECTORY
                      ${CMAKE_CURRENT_BINARY_DIR}/test)

add_dependencies(TestToolbox googletest)
target_link_libraries(TestToolbox ${GTEST_LIBRARIES} libtoolbox)

add_test(NAME TestToolbox
         WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/test
         COMMAND ${CMAKE_CURRENT_BINARY_DIR}/test/TestToolbox)
