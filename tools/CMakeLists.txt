# Tools - Cmake

include_directories( ${FREETYPE_INCLUDE_DIRS} 
					 ${PNG_PNG_INCLUDE_DIR} )

# ----------------------------------------------------------------------------
# fontpack utility
# ----------------------------------------------------------------------------
SET ( fontpack_SRCS
	fontpack/fontpack.cpp
	 )	

add_executable( fontpack ${fontpack_SRCS} )
set_target_properties( fontpack PROPERTIES
			RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/tools/fontpack )

target_link_libraries( fontpack 
						${FREETYPE_LIBRARIES}
						${PNG_LIBRARY} 
						${LIBS} )
