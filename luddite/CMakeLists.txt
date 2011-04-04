

SET ( core_SRCS 

      core/allocator.cpp
      core/debug.cpp

      )

# The main game library
add_library( luddite ${core_SRCS} )
