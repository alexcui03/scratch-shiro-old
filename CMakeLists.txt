cmake_minimum_required(VERSION 3.15)

project(clipcc-runtime LANGUAGES CXX)
set(CMAKE_CXX_STANDARD_REQUIRED true)
set(CMAKE_CXX_STANDARD 20)

set(PRJ_SRC)
set(PRJ_LIB)
set(PRJ_INC)

file(GLOB HEADER_FILES ${CMAKE_SOURCE_DIR}/vm/*.h)
file(GLOB PRJ_SRC ${CMAKE_SOURCE_DIR}/vm/*.cpp)
list(APPEND PRJ_INC ${CMAKE_SOURCE_DIR}/src)

add_executable(${PROJECT_NAME} ${PRJ_SRC})
target_include_directories(${PROJECT_NAME} PRIVATE ${PRJ_INC})
target_link_libraries(${PROJECT_NAME} PRIVATE ${PRJ_LIB})
