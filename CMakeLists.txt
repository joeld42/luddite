CMAKE_MINIMUM_REQUIRED( VERSION 2.6.4 )

set ( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake/Modules)

# Add luddite to the project include/link
project( luddite )
include_directories( . )
link_directories( ${luddite_BINARY_DIR}/luddite )

# Look for dependancies
FIND_PACKAGE( OpenGL )
FIND_PACKAGE( SDL )

#FIND_PACKAGE( PNG )

# Add EASTL
include_directories( extern/eastl/include )
link_directories( ${luddite_BINARY_DIR}/eastl )
ADD_SUBDIRECTORY( extern/eastl )

# Main Library
ADD_SUBDIRECTORY( luddite )

# Examples
ADD_SUBDIRECTORY( example/basics )
