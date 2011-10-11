#ifndef CHIP_H
#define CHIP_H
//
//  chip.h
//  luddite
//
//  Created by Joel Davis on 4/13/11.
//  Copyright 2011 Joel Davis. All rights reserved.
//

#include <ft2build.h>
#include FT_FREETYPE_H

#include "image.h"

enum 
{
    ChipRole_GLYPH  = 0x01,   // Chip will be included in font exports 
    ChipRole_SPRITE = 0x02    // Chip will be included in sprite exports
};

// ===========================================================================
struct Chip
{   
    // role
    size_t m_role;
    
    // glyph info
    char m_char;
    int m_pxlsize;    
    int m_baseline;
    
    // sprite info
    char *m_spriteName;
    
    // layout info
    int m_xpos, m_ypos;
    int m_width, m_height;
    
    // the image data for this chip
    FpImage *m_img;
    
    // constructors
    static Chip *makeGlyph( FT_Library *ft, FT_Face face,
                            int ch, int borderWidth,
                            unsigned long fgColor = 0xFFFFFFFF,
                            unsigned long bgColor = 0x00000000,
                            unsigned long borderColor = 0xFF000000 );
    
    // an "extra" chip is a blank glyph with an outline and
    // the code drawn into it. It is intended that in the final font
    // image, special images (such as a heart or coin, etc..) will be
    // drawn replacing the chip. 
    //
    static Chip *makeExtra( int ch, int w, int h, int pxlsize,
                            unsigned long fgColor=0xFF8888AA,
                            unsigned long bgColor=0xFF444488 );
    
    
    // An "extra" chip can be added by packing an image directly. This
    // can be used for sprites or HUD elements or similar
    /// TOD
//    static Chip *makeExtra( int ch, int pxlsize,
//                           FpImage *srcImg );
    
    // Pack a sprite
    static Chip *makeSprite( const char *spriteName, FpImage *srcImg );
    
    
};

#endif
