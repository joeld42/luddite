include_directories( . )

SET( common_SRCS
      common/allocator.cpp
      common/debug.h
      common/debug.cpp
)
SOURCE_GROUP( common FILES ${common_SRCS} )

SET( game_SRCS 
	game/gameloop.h
	game/gameloop.cpp
)
SOURCE_GROUP( game FILES ${game_SRCS} )

SET( render_SRCS
	render/color_util.cpp
	render/color_util.h
	render/drawvert.h
	render/gbatch.cpp
	render/gbatch.h
	render/gbuff.cpp
	render/gbuff.h
	render/gbuff_prim.cpp
	render/gbuff_prim.h
	render/material.cpp
	render/material.h
	render/material_db.cpp
	render/material_db.h
	render/param.cpp
	render/param.h
	render/render_device.cpp
	render/render_device.h

	render/render_device_gl.cpp
	render/render_device_gl.h

#	render/render_device_es2.cpp
#	render/render_device_es2.h

	render/scene.cpp
	render/scene.h
	render/scene_node.cpp
	render/scene_node.h
	render/scene_objfile.cpp
	render/scene_objfile.h
	render/shader.h
	render/shader.cpp
	render/texture_info.h
	render/texture_info.cpp
)
SOURCE_GROUP( render FILES ${render_SRCS} )

SET( platform_SRCS
      platform/platform.h
      platform/osx/osx_debug.cpp
      platform/osx/osx_gfxutil.cpp
)
SOURCE_GROUP( platform FILES ${platform_SRCS} )

# The main game library
add_library( luddite ${common_SRCS} ${game_SRCS} ${render_SRCS} ${platform_SRCS} )
