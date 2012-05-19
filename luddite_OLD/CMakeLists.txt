

SET ( core_SRCS 

      core/allocator.cpp
      core/debug.h
      core/debug.cpp
      core/texture.h
      core/texture.cpp
      core/singleton.h
      core/gbuff.h
      core/resource.h


      # FIXME: make this choose a different
      # platform file based on, well, platform
      platform/platform.h
      platform/platform_SDL.cpp
      platform/GLee.h
      platform/GLee.c
      platform/gl.h
      platform/platform_gl.cpp

      )

# The main game library
add_library( luddite ${core_SRCS} )
