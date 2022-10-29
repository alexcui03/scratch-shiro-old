cmake_minimum_required(VERSION 3.15)

project(clipcc-runtime LANGUAGES CXX)
set(CMAKE_CXX_STANDARD_REQUIRED true)
set(CMAKE_CXX_STANDARD 20)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_compile_options(-fcoroutines -fpermissive)
endif()

############### compiler ###############

# third_party/libzip
set(LIBZIP_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third_party/libzip)
set(ENABLE_BZIP2 OFF)
set(ENABLE_LZMA OFF)
set(ENABLE_ZSTD OFF)
set(BUILD_DOC OFF)
set(BUILD_EXAMPLES OFF)
set(BUILD_REGRESS OFF)
set(BUILD_TOOLS OFF)
add_subdirectory(${LIBZIP_DIR})
#get_target_property(LIBZIP_INC zip INTERFACE_INCLUDE_DIRECTORIES)
set(LIBZIP_INC ${CMAKE_CURRENT_BINARY_DIR}/third_party/libzip ${LIBZIP_DIR}/lib)

# third_party/jsoncpp
set(JSONCPP_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third_party/jsoncpp)
set(JSONCPP_WITH_TESTS OFF)
add_subdirectory(${JSONCPP_DIR})
get_target_property(JSONCPP_INC jsoncpp_static INTERFACE_INCLUDE_DIRECTORIES)

# third_party/argparse
set(ARGPARSE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third_party/argparse)
set(ARGPARSE_INC ${ARGPARSE_DIR})

# clipcc-compiler
set(COMPILER_SRC)
set(COMPILER_INC)
set(COMPILER_LIB jsoncpp_static zip)

file(GLOB COMPILER_INC ${CMAKE_CURRENT_SOURCE_DIR}/compiler/*.h)
file(GLOB COMPILER_SRC ${CMAKE_CURRENT_SOURCE_DIR}/compiler/*.cpp)
list(APPEND COMPILER_INC ${JSONCPP_INC} ${ARGPARSE_INC} ${LIBZIP_INC})

add_executable(clipcc-compiler ${COMPILER_SRC})
target_include_directories(clipcc-compiler PRIVATE ${COMPILER_INC})
target_link_libraries(clipcc-compiler PRIVATE ${COMPILER_LIB})

############### vm ###############

# clipcc-vm
set(VM_SRC)
set(VM_INC)
set(VM_LIB)

file(GLOB VM_INC ${CMAKE_CURRENT_SOURCE_DIR}/vm/*.h)
file(GLOB VM_SRC ${CMAKE_CURRENT_SOURCE_DIR}/vm/*.cpp)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    list(APPEND VM_LIB pthread)
endif()

#add_library(clipcc-vm SHARED ${VM_SRC})
#target_include_directories(clipcc-vm PUBLIC ${VM_INC})
#target_link_libraries(clipcc-vm PUBLIC ${VM_LIB})

#add_library(clipcc-vm-static STATIC ${VM_SRC})
#target_include_directories(clipcc-vm-static PUBLIC ${VM_INC})
#target_link_libraries(clipcc-vm-static PUBLIC ${VM_LIB})

############### player ###############

# glfw
set(GLFW_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third_party/glfw)
set(GLFW_INC ${GLFW_DIR}/include)
set(GLFW_LIB ${GLFW_DIR}/lib/glfw3.lib)

# glad
set(GLAD_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third_party/glad)
set(GLAD_INC ${GLAD_DIR}/include)
set(GLAD_SRC ${GLAD_DIR}/src/glad.c)

# others (glm, stb)
set(OTHER_INC ${CMAKE_CURRENT_SOURCE_DIR}/third_party)

# player
set(PLAYER_SRC)
set(PLAYER_INC)
set(PLAYER_LIB ${GLFW_LIB})

file(GLOB PLAYER_INC ${CMAKE_CURRENT_SOURCE_DIR}/player/*.h)
file(GLOB PLAYER_SRC ${CMAKE_CURRENT_SOURCE_DIR}/player/*.cpp)
list(APPEND PLAYER_INC ${OTHER_INC} ${GLAD_INC} ${GLFW_INC} ${CMAKE_CURRENT_SOURCE_DIR})
list(APPEND PLAYER_SRC ${GLAD_SRC})

add_library(clipcc-player STATIC ${PLAYER_SRC} ${VM_SRC})
target_include_directories(clipcc-player PRIVATE ${PLAYER_INC} ${VM_INC})
target_link_libraries(clipcc-player PRIVATE ${PLAYER_LIB} ${VM_LIB})

############### test ###############
set(TEST_SRC)
set(TEST_LIB clipcc-player)

file(GLOB TEST_SRC ${CMAKE_CURRENT_SOURCE_DIR}/test/example/*.cpp)
list(APPEND TEST_INC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(clipcc-test ${TEST_SRC} ${TEST_INC})
target_include_directories(clipcc-test PRIVATE ${TEST_INC})
target_link_libraries(clipcc-test PRIVATE ${TEST_LIB})

############### other ###############

# tests
enable_testing()
add_subdirectory(test)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/CTestCustom.cmake ${CMAKE_CURRENT_BINARY_DIR})

# configs
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib)
set(CMAKE_PDB_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)
