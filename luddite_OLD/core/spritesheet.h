#ifndef SPRITESHEET_H
#define SPRITESHEET_H

#include <EASTL/vector.h>

#include <luddite/core/debug.h>
#include <luddite/core/gbuff.h>
#include <luddite/core/texture.h>

namespace luddite 
{

// SpriteSheet is a collection of sprites that share a texture.
class Sprite;


// ========================================================================
class SpriteSheet : public GBuff<SpriteVert>
{
public:
    SpriteSheet( luddite::HTexture hTex );

    Sprite *makeSprite( float s0=0.0, float t0=0.0,
                        float s1=1.0, float t1=1.0 );
    
    void removeSprite( Sprite *spr );
    void removeAll();

private:
    luddite::HTexture m_hSheetTex;  
    eastl::vector<Sprite*> m_sprites;    
};

}; // namespace luddite
 

#endif

