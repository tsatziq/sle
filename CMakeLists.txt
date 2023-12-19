cmake_minimum_required(VERSION 3.14)

project(sle VERSION 0.1)
set(CMAKE_BUILD_TYPE Debug)

# find packages
find_package(Curses REQUIRED)

# create targets
add_executable(sle src/main.cpp)
add_library(common "")
add_subdirectory(src)

target_include_directories(common PUBLIC
    "${PROJECT_SOURCE_DIR}/include"
    ${CURSES_INCLUDE_DIRS})

target_link_libraries(sle PUBLIC common ${CURSES_LIBRARIES})
target_compile_features(common PRIVATE cxx_std_17)
target_compile_features(sle PRIVATE cxx_std_17)
