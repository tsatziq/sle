cmake_minimum_required(VERSION 3.14)

project(sle VERSION 0.1)

# find packages
find_package(Curses REQUIRED)

# create targets
add_executable(sle src/main.cpp)
add_library(common "")
add_subdirectory(src)

# probably dont need this, include in common as public instead
#target_include_directories(sle PUBLIC
#    "${PROJECT_SOURCE_DIR}/include")

target_include_directories(common PUBLIC
    "${PROJECT_SOURCE_DIR}/include"
    ${CURSES_INCLUDE_DIRS})

#target_link_libraries(common PUBLIC )
target_link_libraries(sle PUBLIC common ${CURSES_LIBRARIES})
