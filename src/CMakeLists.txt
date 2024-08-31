# src subdirectory CMakeLists.txt

target_sources(common
    PRIVATE
        buffer.cpp
        ${CMAKE_SOURCE_DIR}/include/sle/context.h
        ${CMAKE_SOURCE_DIR}/include/sle/normalmodelexer.h
        editloop.cpp
        filehandler.cpp
        insertmode.cpp
    	mainscreen.cpp
        normalmode.cpp
        ${FLEX_normalModeLexer_OUTPUTS}
)

