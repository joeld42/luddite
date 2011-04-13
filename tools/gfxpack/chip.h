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

// ===========================================================================
struct Chip
{    
    // glyph info
    char m_char;
    int m_baseline;
    
    // sprite info
    // TODO
    
    // layout info
    int m_xpos, m_ypos;
    int m_width, m_height;
    
    // the image data for this chip
    FpImage *m_img;
    
    // constructors
    static Chip *makeGlyph( FT_Library *ft, FT_Face face,
                           int ch, int borderWidth );
};
