# src subdirectory CMakeLists.txt

target_sources(common
    PRIVATE
        buffer.cpp
        ${CMAKE_SOURCE_DIR}/include/sle/context.h
        ${CMAKE_SOURCE_DIR}/include/sle/cmdmodelexer.h
        ${CMAKE_SOURCE_DIR}/include/sle/debugger.h
        cmdline.cpp
        cmdmode.cpp
        editloop.cpp
        filehandler.cpp
        insertmode.cpp
    	mainscreen.cpp
        normalmode.cpp
        ${FLEX_normalModeLexer_OUTPUTS}
)

