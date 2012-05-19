#include <luddite/core/debug.h>
#include <luddite/core/spritesheet.h>

using namespace luddite;

// ========================================================================
SpriteSheet::SpriteSheet( luddite::HTexture hTex ) :
    GBuff<SpriteVert>( GL_DYNAMIC_DRAW ),
    m_hSheetTex( hTex )
{    
}

Sprite *SpriteSheet::makeSprite( float s0, float t0,
                                 float s1, flaot t1 )
{
    SpriteVert *newVert;
    newVert = this->addTriangles( 2 );
    
    Sprite *sprite = new Sprite( this, newVert );
    sprite->setTexCoords( s0, t0, s1, t1 );
    
    // call update to initialize pos data
    sprite->update();
    
    // Store this sprite
    m_sprites.push_back( sprite );
    
    // return it
    return sprite;    
}

void SpriteSheet::removeSprite( Sprite *spr )
{
    DBG::info("removeSprite, sprite list size is %d\n", 
              m_sprites.size() );
    
    // Find the sprite with the highest vert data index, i.e.
	// the one on the end of the list
	// FIXME: make this better aaagrrggghh
	Sprite *lastSprite = m_sprites.back();
	for (eastl::vector<Sprite*>::iterator spi = m_sprites.begin();
		 spi != m_sprites.end(); spi++)
	{
		if ((*spi)->m_vertData > lastSprite->m_vertData)
		{
			lastSprite = (*spi);
		}
	}
	
	// relocate the last sprite to take this one's place
	memcpy( spr->m_vertData, lastSprite->m_vertData, sizeof(SpriteVert)*6 ); 
	lastSprite->m_vertData = spr->m_vertData;
	
	// Remove the sprite
	m_sprites.remove( spr );
	
	// And account for this quad's verts
	m_buffSize -= 6;
}


