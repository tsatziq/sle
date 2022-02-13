# src subdirectory CMakeLists.txt

target_sources(common
    PRIVATE
        test.cpp
        dispatcher.cpp
        poll.cpp
    	screenmanager.cpp
    	mainscreen.cpp
    	sidebar.cpp
    	cmdline.cpp
		modeline.cpp
)

