

SET ( core_SRCS 

      core/allocator.cpp
      core/debug.cpp
      #core/texture.cpp

      )

# The main game library
add_library( luddite ${core_SRCS} )
