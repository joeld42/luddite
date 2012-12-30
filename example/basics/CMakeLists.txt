

include_directories(
    ${OPENGL_INCLUDE_DIR}
    ${SDL_INCLUDE_DIR}
    ${PNG_PNG_INCLUDE_DIR}
    )

set ( LIBS ${LIBS}
    ${SDL_LIBRARY}
    ${OPENGL_LIBRARIES}
    ${PNG_LIBRARY}
    )

# ----------------------------------------------------------------------------
# Hello World
# ----------------------------------------------------------------------------
SET ( hello_SRCS
       hello.cpp
	 )	

# the demo executable
add_executable( hello-luddite ${hello_SRCS} )
set_target_properties( hello-luddite PROPERTIES
			RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/example/basics )

target_link_libraries( hello-luddite luddite eastl glsw ${LIBS} )

