//
//  material_db.h
//  luddite_ios
//
//  Created by Joel Davis on 8/20/12.
//  Copyright (c) 2012 Joel Davis. All rights reserved.
//

#ifndef luddite_material_db_h
#define luddite_material_db_h

#include <EASTL/string.h>

namespace luddite
{
    
class Material;
    
class MaterialDB
{

public:
    
    // Material "Database" path for glsw
    void initShaderDB( const char *resourcePath );
    
    // Adds a material definition file, eg "foo.material.xml"
    void addMaterialDefs( const char *materialFile );
    
    // Looks up a named material. If the shader hasn't been instantiated yet,
    // compiles it. TODO: prewarm
    Material *getNamedMaterial( RenderDevice *device, const eastl::string &mtlName );
    
//private:
    Material *_materialWithKey( RenderDevice *device, const eastl::string &mtlKey );
    
    eastl::string m_resourcePath;
  
};

} // namespace luddite
    
#endif
