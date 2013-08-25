include_directories( . )

SET ( testapp_SRCS
  test_scene.h
  test_scene.cpp
  scene_mesh.h
  scene_mesh.cpp
  scene_collect.h
  scene_collect.cpp
  test_shell.h
  test_shell.cpp
  testapp_sdl.cpp
)
SOURCE_GROUP( src FILES ${testapp_SRCS} )

# Include the gamedata files in their own group.
# Note you still need to manually add the "Copy Resources to Bundle"
# and add them there.
SET ( gamedata_FILES
#    gamedata/nesfont.png
#    gamedata/simpletex.png
#    gamedata/testapp.glsl
)
SOURCE_GROUP( gamedata FILES ${gamedata_FILES} )

include_directories( 
		     ${CMAKE_SOURCE_DIR}/extern/
		     ${CMAKE_SOURCE_DIR}/extern/glsw/
         ${CMAKE_SOURCE_DIR}/extern/prmath/
         ${CMAKE_SOURCE_DIR}/extern/eastl/
         ${CMAKE_SOURCE_DIR}/extern/rapidxml/

		     ${OPENGL_INCLUDE_DIR}
		     ${SDL_INCLUDE_DIR}
		     ${PNG_PNG_INCLUDE_DIR}
		     )

set ( LIBS ${LIBS}
      ${OPENGL_LIBRARIES}
      ${SDL_LIBRARY}
      ${PNG_LIBRARY}
      luddite
      eastl
      glsw
      ) 

ADD_DEFINITIONS( ${PNG_DEFINITIONS} )

#IF(APPLE)
#   SET( MACOSX_BUNDLE_ICON_FILE testapp.icns 
#ENDIF(APPLE)

ADD_EXECUTABLE( testapp MACOSX_BUNDLE ${testapp_SRCS} ${gamedata_FILES} )
target_link_libraries( testapp ${LIBS} )
set_target_properties( testapp PROPERTIES
		       		 RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/example/testapp/ )


#INSTALL( TARGETS takethis
#   BUNDLE_DESTINATION . COMPONENT Runtime
#  RUNTIME DESTINATION bin COMPONENT Runtime
#    )

#set(APPS ...)  # paths to executables
#set(DIRS ...)   # directories to search for prerequisites

#message ( "APPS \${CMAKE_INSTALL_PREFIX}/${_dir}/${PROJECT_NAME}${EXE_DEBUG_EXTENSION}${_extension}" )

#INSTALL(CODE "
 #  include(BundleUtilities)
  # fixup_bundle(\"${APPS}\"   \"\"   \"${DIRS}\")
  # " COMPONENT Runtime)
