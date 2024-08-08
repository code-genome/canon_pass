project(canonicalization-pass)
SET(CMAKE_CXX_FLAGS "-Wall -fno-rtti")

set(CMAKE_BUILD_TYPE Release)

cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
find_package(LLVM 8.0.1 EXACT REQUIRED CONFIG)

message(STATUS "Building the LLVM Canonicalization Pass")
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")
add_definitions(${LLVM_DEFINITIONS})
include_directories(${LLVM_INCLUDE_DIRS})
add_library(canonicalization-pass MODULE canonicalization.cpp)

message(STATUS "Building the testing executable")
include_directories(${LLVM_INCLUDE_DIRS})
separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS_LIST})
add_executable(canonicalization_test canonicalization_test.cpp canonicalization.cpp)
# Find the libraries that correspond to the LLVM components
# that we wish to use
llvm_map_components_to_libnames(llvm_libs support core irreader)
# Link against LLVM libraries
target_link_libraries(canonicalization_test ${llvm_libs})
