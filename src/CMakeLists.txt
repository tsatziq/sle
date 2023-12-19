# src subdirectory CMakeLists.txt

target_sources(common
    PRIVATE
        buffer.cpp
        ${CMAKE_SOURCE_DIR}/include/sle/context.h
        editloop.cpp
        filehandler.cpp
    	mainscreen.cpp
)

