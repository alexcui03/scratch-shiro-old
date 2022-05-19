cmake_minimum_required(VERSION 3.15)

project(clipcc-runtime LANGUAGES CXX)
set(CMAKE_CXX_STANDARD_REQUIRED true)
set(CMAKE_CXX_STANDARD 20)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_compile_options(-fcoroutines -fpermissive)
endif()

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
set(VM_SRC)
set(VM_INC)
set(VM_LIB)

file(GLOB VM_INC ${CMAKE_SOURCE_DIR}/vm/*.h)
file(GLOB VM_SRC ${CMAKE_SOURCE_DIR}/vm/*.cpp)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    list(APPEND VM_LIB pthread)
endif()

add_library(clipcc-vm SHARED ${VM_SRC})
target_include_directories(clipcc-vm PUBLIC ${VM_INC})
target_link_libraries(clipcc-vm PUBLIC ${VM_LIB})

add_library(clipcc-vm-static STATIC ${VM_SRC})
target_include_directories(clipcc-vm-static PUBLIC ${VM_INC})
target_link_libraries(clipcc-vm-static PUBLIC ${VM_LIB})

# tests
enable_testing()
set(TEST_FILES)
file(GLOB TEST_FILES ${CMAKE_SOURCE_DIR}/test/*.json)
foreach(TEST_FILE ${TEST_FILES})
    string(REGEX MATCH "([a-zA-Z0-9]*).json" TEST_NAME ${TEST_FILE})
    string(REGEX REPLACE ".json" "" TEST_NAME ${TEST_NAME})
    add_test(NAME ${TEST_NAME} COMMAND clipcc-compiler -s ${TEST_FILE})
endforeach()

configure_file(${CMAKE_SOURCE_DIR}/CTestCustom.cmake ${CMAKE_BINARY_DIR})

# configs
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_PDB_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
