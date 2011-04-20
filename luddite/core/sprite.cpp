//
//  sprite.cpp
//  luddite
//
//  Created by Joel Davis on 4/19/11.
//  Copyright 2011 Joel Davis. All rights reserved.
//

#include <math.h>
#include <stdlib.h>

#include <luddite/core/sprite.h>
#include <luddite/core/spritesheet.h>

using namespace luddite;

Sprite::Sprite( GBuff<SpriteVert> *sheet, SpriteVert *vertData ) :
    m_sheet( sheet ),
    m_vertData( vertData ),
    x(0.0f), y(0.0f),
    angle( 0.0f ), 
    sx( 1.0 ), sy(1.0)
{
}

Sprite::~Sprite()
{
	m_sheet->removeSprite( this );
}	

// set st pos
void Sprite::setTexCoords( float s0, float t0,
						   float s1, float t1 )
{
	// Upper tri
	m_vertData[0].st[0]  =  s0; m_vertData[0].st[1]  =  t1;
	m_vertData[1].st[0]  =  s1; m_vertData[1].st[1]  =  t0;
	m_vertData[2].st[0]  =  s0; m_vertData[2].st[1]  =  t0;
	
	// Lower Tri
	m_vertData[3].st[0]  =  s0; m_vertData[3].st[1]  =  t1;
	m_vertData[4].st[0]  =  s1; m_vertData[4].st[1]  =  t0;
	m_vertData[5].st[0]  =  s1; m_vertData[5].st[1]  =  t1;
	
}


void Sprite::flipHoriz()
{
	m_vertData[0].st[0] = m_vertData[5].st[0];
	m_vertData[4].st[0] = m_vertData[2].st[0];
	
	std::swap( m_vertData[2].st[0], m_vertData[1].st[0] );
	std::swap( m_vertData[3].st[0], m_vertData[5].st[0] );
	
}

void Sprite::update()
{
	float ca = cos( angle * SPRITE_D2R );
	float sa = sin( angle * SPRITE_D2R );
	float sx2 = sx * 0.5f;
	float sy2 = sy * 0.5f;
	
	// Upper Tri
	m_vertData[0].pos[0] = x + ((-sx2)*ca - (-sy2)*sa);
	m_vertData[0].pos[1] = y + ((-sy2)*ca + (-sx2)*sa);
    
	m_vertData[1].pos[0] = x + (( sx2)*ca - ( sy2)*sa);
	m_vertData[1].pos[1] = y + (( sy2)*ca + ( sx2)*sa);
	
	m_vertData[2].pos[0] = x + ((-sx2)*ca - ( sy2)*sa);
	m_vertData[2].pos[1] = y + (( sy2)*ca + (-sx2)*sa);
    
	// Lower Tri
	m_vertData[3].pos[0] = x + ((-sx2)*ca - (-sy2)*sa);
	m_vertData[3].pos[1] = y + ((-sy2)*ca + (-sx2)*sa);
	
	m_vertData[4].pos[0] = x + (( sx2)*ca - ( sy2)*sa);
	m_vertData[4].pos[1] = y + (( sy2)*ca + ( sx2)*sa);
	
	m_vertData[5].pos[0] = x + (( sx2)*ca - (-sy2)*sa);
	m_vertData[5].pos[1] = y + ((-sy2)*ca + ( sx2)*sa);
}

// Tests if a point hits us
bool Sprite::testHit( float tx, float ty )
{
	float px, py;
	
	// Unrotate the point to align with the bbox
	tx = tx - x;
	ty = ty - y;
	
	float ca = cos( -angle * SPRITE_D2R );
	float sa = sin( -angle * SPRITE_D2R );
	
	px = (tx*ca) - (ty*sa);
	py = (ty*ca) + (tx*sa);
	
	// Test against sprite
	float sx2 = sx / 2;
	float sy2 = sy / 2;
	if ( (px >= -sx2) && 
        (px <= sx2) &&
        (py >= -sy2) &&
        (py <= sy2 ) )
	{
		// we hit the sprite
		return true;
	}
	
	return false;
}
