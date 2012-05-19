include_directories( . )

SET( common_SRCS
      common/allocator.cpp
      common/debug.h
      common/debug.cpp
)
SOURCE_GROUP( common FILES ${common_SRCS} )

SET( platform_SRCS

      platform/platform.h

      platform/osx/osx_debug.cpp
)
SOURCE_GROUP( platform FILES ${platform_SRCS} )

# The main game library
add_library( luddite ${common_SRCS} ${platform_SRCS} )
