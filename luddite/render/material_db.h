
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
#include <EASTL/hash_map.h>

namespace luddite
{
    
class Material;
class Shader;

class TextureInfo;

class MaterialDB
{

public:
    
    // Material "Database" path for glsw
    void initShaderDB(); // use default resource path
    void initShaderDB( const char *resourcePath );
    
    // Adds a material definition file, eg "foo.material.xml"
    void addMaterialDefs( const char *materialFile );
    
    // Looks up a named material. If the shader hasn't been instantiated yet,
    // compiles it. TODO: prewarm
    Material *getNamedMaterial( RenderDevice *device, const eastl::string &mtlName );

    // Loads all shaders used by any material. Probably not the best, need to figure
    // out what materials we're actually using and use that. But for now this works
    void useAllShaders( RenderDevice *device );
    
//private:
    Material *_materialWithKey( const eastl::string &mtlKey );
    Material *_lookupMaterial( const eastl::string &mtlName );

    luddite::TextureInfo *_lookupTexture( const eastl::string &filename );

    void _parseParam( Material *mtl, const eastl::string &paramName, const char *value );
    
    eastl::string m_resourcePath;
    eastl::hash_map<eastl::string,luddite::Material*> m_materials;
    eastl::hash_map<eastl::string,Shader*> m_shaders; // shader programs, indexed by shaderKey

    // store textures
    eastl::hash_map<eastl::string,TextureInfo*> m_textures;
};

} // namespace luddite
    
#endif
