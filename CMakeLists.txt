cmake_minimum_required(VERSION 3.28)

project(allocators VERSION 1.0.0 LANGUAGES C)

set(CMAKE_ENABLE_EXPORTS TRUE)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_C_FLAGS_DEBUG "-ggdb -O0")
set(CMAKE_C_FLAGS_RELEASE "-Wall -O3")

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/**.c)

add_executable(allocators ${SOURCES})

target_include_directories(allocators PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
