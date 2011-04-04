# Source Files
SET ( eastl_SRCS
      src/allocator.cpp
      src/red_black_tree.cpp
      src/hashtable.cpp
      src/assert.cpp
      src/fixed_pool.cpp
      src/string.cpp )

# EASTL library
add_library( eastl ${eastl_SRCS} )

