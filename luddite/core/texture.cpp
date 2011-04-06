#include <string.h>

#include <luddite/core/texture.h>
#include <luddite/core/debug.h>

#include <luddite/platform/platform.h>

using namespace luddite;

// adapted from
// http://graphics.stanford.edu/~seander/bithacks.html
bool isPow2(unsigned int v)
{    
    unsigned int c; // c accumulates the total bits set in v
    for (c = 0; v; c++)
    {
        v &= v - 1; // clear the least significant bit set
    }

    return (c==1);    
}



//  entry points for the resource manager
bool loadResource( const char *filename, luddite::TextureGL *texture )
{
    // Remember filename
    texture->m_texName = filename;

    // call platform code to do the heavy lifting
    GLubyte *data = NULL;
    if (platform_loadTexture( filename, texture, &data ))
    {
        DBG::info( "Loaded texture %s\n", filename );
        GLuint textureId = 0;
        glGenTextures( 1, &textureId );
        glBindTexture( GL_TEXTURE_2D, textureId );
        
        // FIXME: make settable
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
        
        glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, 
                      texture->m_width, texture->m_height,
                      0, GL_RGBA, GL_UNSIGNED_BYTE,
                      data );

        texture->m_texId = textureId;
        
        // FIXME: use allocator
        free( data );        
    }
    
    return true;    
}

void unloadResource( luddite::TextureGL *texture )
{
    // Release the texture from opengl
    glDeleteTextures( 1, &(texture->m_texId) );
}

HTexture TextureDB::getTexture( const char *name )
{
    return BaseTextureDB::getResource( name );    
}

#if 0
HTexture TextureDB::buildTextureFromData( const char *name, 
                                          const GLubyte *data, 
                                          GLint width, GLint height )
{
	HTexture hTex = BaseTextureDB::getResource( name, true );

	// texture is not initialized, do so now
	TextureGL *tex = m_resMgr.deref( hTex );

	tex->m_texName = name;
	glGenTextures( 1, &(tex->m_texId) );

	glBindTexture(GL_TEXTURE_2D, tex->m_texId);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
				  GL_RGB, GL_UNSIGNED_BYTE, data );
	
	tex->m_origWidth = width;
	tex->m_origHeight = height;

	tex->m_width = width;
	tex->m_height = height;

	return hTex;
	
}
#endif

void TextureDB::freeTexture( HTexture hTex )
{
    return BaseTextureDB::freeResource( hTex );
}

const eastl::string &TextureDB::getTextureName( HTexture hTex )
{
    TextureGL *tex = m_resMgr.deref( hTex );
    AssertPtr( tex );
    
    return tex->m_texName;    
}


GLuint TextureDB::getTextureId( HTexture hTex )
{
    TextureGL *tex = m_resMgr.deref( hTex );
    AssertPtr( tex );
    
    return tex->m_texId;    
}

#if 0
void TextureDB::reportTexture( const eastl::string &resName, unsigned int refCount, HTexture hTex )
{
	
	TextureDB &texDB = TextureDB::singleton();
	TextureGL *tex = texDB.m_resMgr.deref( hTex );

	DBG::info( "%10s|%3d|%4dx%-4d", resName.c_str(), refCount, 
				tex->m_width, tex->m_height  );
}

void TextureDB::reportUsage()
{
	DBG::info("=======================================\n" );
	DBG::info(" Texture Usage\n" );
	DBG::info("=======================================\n" );

	doReport( reportTexture );
}
#endif

