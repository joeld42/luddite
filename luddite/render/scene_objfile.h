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
#include "material_db.h"

namespace luddite
{

// should make it load from a buffer not a file
    SceneNode *scene_objfile(char const *filename, RenderDevice *renderDevice, MaterialDB *mtlDB);
    
} // namespace luddite

#endif
