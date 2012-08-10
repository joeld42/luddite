//
//  scene_objfile.h
//  luddite_ios
//
//  Created by Joel Davis on 8/10/12.
//  Copyright (c) 2012 Joel Davis. All rights reserved.
//

#ifndef luddite_scene_objfile_h
#define luddite_scene_objfile_h

#include <EASTL/list.h>
#include <luddite/render/scene.h>
#include <luddite/render/scene_node.h>

namespace luddite
{

// should make it load from a buffer not a file
luddite::SceneNode *scene_objfile( const char *filename );
    
} // namespace luddite

#endif
