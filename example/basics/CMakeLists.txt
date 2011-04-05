

include_directories(
    ${OPENGL_INCLUDE_DIR}
    ${SDL_INCLUDE_DIR}
    )

set ( LIBS ${LIBS}
    ${SDL_LIBRARY}
    ${OPENGL_LIBRARIES}
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

target_link_libraries( hello-luddite luddite eastl ${LIBS} )


# ----------------------------------------------------------------------------
# Resource Test
# 
# Basic example of how to create a custom type of managed resource
# ----------------------------------------------------------------------------
SET ( resource_test_SRCS
	resource_test.cpp
	 )	

# the demo executable
add_executable( resource-test ${resource_test_SRCS} )
set_target_properties( resource-test PROPERTIES
			RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/example/basics )

target_link_libraries( resource-test luddite eastl )
