#ifndef TEXTURE_H
#define TEXTURE_H


#include <EASTL/string.h>

#ifdef WIN32
# define WIN32_LEAN_AND_MEAN
# include <winsock2.h>
# include <windows.h>
# include <crtdbg.h>
#endif

#include <luddite/platform/gl.h>

#include <luddite/core/handle.h>
#include <luddite/core/resource.h>
#include <luddite/core/singleton.h>

namespace luddite
{

// ===========================================================================
struct TextureGL
{
    eastl::string m_texName;

	GLuint m_width;    // width bumped to pow2
	GLuint m_height;

	GLuint m_origWidth;  // orig width
	GLuint m_origHeight;

    GLuint m_texId;
};

// Typedefs
struct tag_TEXTURE {};
typedef Handle<tag_TEXTURE> HTexture;
typedef HandleMgr<TextureGL,HTexture> HTextureMgr;

// ===========================================================================
typedef ResourceMgr<TextureGL, HTexture> BaseTextureDB;

// All the template funkiness is hidden now. :)
class TextureDB : public BaseTextureDB, public luddite::Singleton<TextureDB>
{
public:

    // Getting/releasing of textures
    HTexture getTexture( const char *name );
    void freeTexture( HTexture hTex );

#if 0
	HTexture buildTextureFromData( const char *name, const GLubyte *data, 
									GLint width, GLint height );
#endif

    // Access to texture
    // TODO: needs to be reconsiled with the 
    // GraphicsES1/GraphicsES2 code
    GLuint getTextureId( HTexture hTex );    

    // Access to texture name
    const eastl::string &getTextureName( HTexture hTex );
	
	// Access to texture size
	// todo
	
#if 0
	// Print texture usage report
	static void reportTexture( const eastl::string &resName, unsigned int refCount, HTexture hTex );
	void reportUsage();
#endif
    
};

}; // namespace 'luddite'
    
// Load/unload functions to use. Thesea are called by the
// TextureDB, and you probably don't need to call them 
// directly
bool loadResource( const char *filename, luddite::TextureGL *texture );
void unloadResource( luddite::TextureGL *texture );

#endif

