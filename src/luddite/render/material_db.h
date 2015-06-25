
//
//  material_db.h
//  luddite_ios
//
//  Created by Joel Davis on 8/20/12.
//  Copyright (c) 2012 Joel Davis. All rights reserved.
//

#ifndef luddite_material_db_h
#define luddite_material_db_h

#include <string>
#include <unordered_map>

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
    Material *getNamedMaterial( RenderDevice *device, const std::string &mtlName );

    // Loads all shaders used by any material. Probably not the best, need to figure
    // out what materials we're actually using and use that. But for now this works
    void useAllShaders( RenderDevice *device );
    
    // prints out the material db for debugging
    void dumpMaterialDB();
    
//private:
    Material *_materialWithKey( const std::string &mtlKey );
    Material *_lookupMaterial( const std::string &mtlName );

    luddite::TextureInfo *_lookupTexture( const std::string &filename );

    void _parseParam( Material *mtl, const std::string &paramName, const char *value, bool objectSpace );
    
    std::string m_resourcePath;
    std::unordered_map<std::string,luddite::Material*> m_materials;
    std::unordered_map<std::string,Shader*> m_shaders; // shader programs, indexed by shaderKey

    // store textures
    std::unordered_map<std::string,TextureInfo*> m_textures;
};

} // namespace luddite
    
#endif
