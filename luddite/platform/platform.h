#ifndef PLATFORM_H
#define PLATFORM_H

#include <luddite/core/texture.h>

// All platform specific code should be isolated here

// ---------------------------------------------------------------------------
// Load a texture into the TextureGL structure
// FIXME: sort out how to deal with platform-specific formats
// FIXME: use allocator for out_pixelData
bool platform_loadTexture( const char *filename, 
                           luddite::TextureGL *texture,
                           unsigned char **out_pixelData );



#endif
