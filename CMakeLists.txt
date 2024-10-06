cmake_minimum_required(VERSION 3.14)

project(sle VERSION 0.1)
set(CMAKE_BUILD_TYPE Debug)

# find packages
find_package(Curses REQUIRED)
find_package(FLEX REQUIRED)

FLEX_TARGET(normalModeLexer
    src/cmdmodelex.l
    "${CMAKE_CURRENT_BINARY_DIR}/cmdmodelexer.cpp")

# create targets
add_executable(sle src/main.cpp)
add_library(common "")
add_subdirectory(src)

target_include_directories(common PUBLIC
    "${PROJECT_SOURCE_DIR}/include"
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    "${CMAKE_CURRENT_BINARY_DIR}"
    ${CURSES_INCLUDE_DIRS})

target_link_libraries(sle PUBLIC common ${CURSES_LIBRARIES} panel)
target_compile_features(common PRIVATE cxx_std_17)
target_compile_features(sle PRIVATE cxx_std_17)
