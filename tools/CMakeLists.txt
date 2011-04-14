# Tools - Cmake

include_directories( ${FREETYPE_INCLUDE_DIRS} 
					 ${PNG_PNG_INCLUDE_DIR} )

# ----------------------------------------------------------------------------
# gfxpack utility
# ----------------------------------------------------------------------------
SET ( gfxpack_SRCS
	 gfxpack/gfxpack.cpp
     gfxpack/chip.cpp
     gfxpack/image.cpp
	 )	

add_executable( gfxpack ${gfxpack_SRCS} )
set_target_properties( gfxpack PROPERTIES
			RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/tools/gfxpack )

target_link_libraries( gfxpack 
						${FREETYPE_LIBRARIES}
						${PNG_LIBRARY} 
						${LIBS} )
