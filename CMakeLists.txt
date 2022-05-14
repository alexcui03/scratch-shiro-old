cmake_minimum_required(VERSION 3.15)

project(clipcc-runtime LANGUAGES CXX)
set(CMAKE_CXX_STANDARD_REQUIRED true)
set(CMAKE_CXX_STANDARD 20)

# third_party/jsoncpp
set(JSONCPP_DIR ${CMAKE_SOURCE_DIR}/third_party/jsoncpp)
set(JSONCPP_WITH_TESTS OFF)
add_subdirectory(${JSONCPP_DIR})
get_target_property(JSONCPP_INC jsoncpp_lib INTERFACE_INCLUDE_DIRECTORIES)

# third_party/argparse
set(ARGPARSE_DIR ${CMAKE_SOURCE_DIR}/third_party/argparse)
set(ARGPARSE_INC ${ARGPARSE_DIR})

# clipcc-compiler
set(COMPILER_SRC)
set(COMPILER_INC)
set(COMPILER_LIB jsoncpp_lib)

file(GLOB COMPILER_INC ${CMAKE_SOURCE_DIR}/compiler/*.h)
file(GLOB COMPILER_SRC ${CMAKE_SOURCE_DIR}/compiler/*.cpp)
list(APPEND COMPILER_INC ${JSONCPP_INC} ${ARGPARSE_INC})

add_executable(clipcc-compiler ${COMPILER_SRC})
target_include_directories(clipcc-compiler PRIVATE ${COMPILER_INC})
target_link_libraries(clipcc-compiler PRIVATE ${COMPILER_LIB})

# clipcc-vm
