#ifndef SPRITE_H
#define SPRITE_H
//
//  sprite.h
//  luddite
//
//  Created by Joel Davis on 4/19/11.
//  Copyright 2011 Joel Davis. All rights reserved.
//

#include <luddite/core/debug.h>
#include <luddite/core/gbuff.h>
#include <luddite/core/texture.h>

namespace luddite
{
    
class SpriteSheet;

// ========================================================================
struct SpriteVert
{
    float m_pos[2];
    float m_st[2];
    
    static void bind()
    {
        BIND_VERTEX( SpriteVert, m_pos );
        BIND_TEXTURE_COORD( SpriteVert, m_st );        
    }    
    
    MAKE_BINDABLE( SpriteVert );
};

// ========================================================================    
class Sprite
{
public:
	Sprite( SpriteSheet *owner, SpriteVert *vertData );
	~Sprite();
	
	// Center position, rotation, size
	float x, y;
	float angle;
	float sx, sy;
	
	// Updates vert data if pos/angle/sz change
	void update();
	
	// Test if a point hits us
	bool testHit( float x, float y );
	
	// set st coords
	void setTexCoords( float s0, float t0,
                       float s1, float t1 );
	
	// Flips texture coords horizontally
	void flipHoriz();
	
	// Buffer owning all sprites that share this texture
    luddite::SpriteSheet *m_sheet;
	
	// Pointer into Spritebuff
	SpriteVert *m_vertData;
};

}; // namespace luddite

#endif