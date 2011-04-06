

SET ( core_SRCS 

      core/allocator.cpp
      core/debug.cpp
      core/texture.cpp

      # FIXME: make this choose a different
      # platform file based on, well, platform
      platform/platform_SDL.cpp

      )

# The main game library
add_library( luddite ${core_SRCS} )
