CMAKE_MINIMUM_REQUIRED( VERSION 2.6.4 )

set ( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake/Modules)

# Add luddite to the project include/link
project( luddite )

message ("CMAKE_ROOT is ${CMAKE_ROOT}")
message ("CMAKE_SOURCE_DIR is ${CMAKE_SOURCE_DIR}" )
message ("Module path is  ${CMAKE_MODULE_PATH} " )

# Look for dependancies
FIND_PACKAGE( OpenGL )
FIND_PACKAGE( SDL )
FIND_PACKAGE( SDL_image )
FIND_PACKAGE( PNG )
FIND_PACKAGE( Freetype )

include_directories( . )
link_directories( ${luddite_BINARY_DIR}/luddite )

# =========================================================
#  extern libs included in the project
# =========================================================

# Add prmath (header only)
include_directories( extern )
include_directories( extern/prmath )

# Add RapidXML (header only)
include_directories( extern/rapidxml )

# Add EASTL
include_directories( extern/eastl/include )
link_directories( ${luddite_BINARY_DIR}/eastl )
ADD_SUBDIRECTORY( extern/eastl )

# Add GLSW
include_directories( extern/glsw )
link_directories( ${luddite_BINARY_DIR}/glsw )
ADD_SUBDIRECTORY( extern/glsw )

# Not using TinyXML at the moment

# =========================================================
#  Luddite code
# =========================================================

# Main Library
ADD_SUBDIRECTORY( luddite )

# Tools
ADD_SUBDIRECTORY( tools )

# Examples
ADD_SUBDIRECTORY( example/basics )
#ADD_SUBDIRECTORY( example/shootymech )
ADD_SUBDIRECTORY( example/testapp )