cmake_minimum_required(VERSION 2.8)
project(flatcloth)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -std=c++14 -Wall -Wextra -pedantic -Weffc++ -O0 -ggdb -fdiagnostics-color=always -fno-omit-frame-pointer -fno-inline -fsanitize=address")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -O2 -fno-rtti")
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" "${PROJECT_SOURCE_DIR}/cmake")

find_package(OpenGL)
find_package(SDL2)
include_directories(${SDL2_INCLUDE_DIR})

file(GLOB_RECURSE SOURCES ./src/*.cc)
file(COPY ./assets/ DESTINATION ./assets/)

add_executable(${CMAKE_PROJECT_NAME} ${SOURCES})
target_link_libraries(${CMAKE_PROJECT_NAME} ${SDL2_LIBRARY})
target_link_libraries(${CMAKE_PROJECT_NAME} -lGL -lGLEW)
