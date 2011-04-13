//
//  chip.cpp
//  luddite
//
//  Created by Joel Davis on 4/13/11.
//  Copyright 2011 Joel Davis. All rights reserved.
//

#include "chip.h"
#include "image.h"

Chip *Chip::makeGlyph(  FT_Library *ft, FT_Face ftFace, int ch )
{
    Chip *chip = new Chip();
    
    int glyph_index = FT_Get_Char_Index( ftFace, ch );	
	FT_GlyphSlot slot = ftFace->glyph;
    
	// draw the glyph without antialiasing to get the char size and
	// to seed the border
	FT_Load_Glyph( ftFace, glyph_index, FT_LOAD_MONOCHROME );
	FT_Render_Glyph( ftFace->glyph, ft_render_mode_mono );
    
    chip->m_xpos = 0;
    chip->m_ypos = 0;
    chip->m_width  = slot->bitmap.width;
    chip->m_height = slot->bitmap.rows;
    chip->m_baseline = slot->bitmap_top;
    
    // TODO: border
	//char_w += borderWidth*2;
	//char_h += borderWidth*2;	    	
    
    // TODO: background color
    chip->m_img = new FpImage( chip->m_width, chip->m_height, 0xFF00FF );	
    
#if 0
    if (borderWidth) {
        
        img->pasteFTBitmap( &slot->bitmap, borderWidth, borderWidth, border, 1 );
        
        for (int b=0; b < borderWidth; b++) {		
            img->thicken( bg );		 
        }
        
    }
	
	// render the glyph again, antialiased, and put it on top of the border
    FT_Load_Glyph( ftFace, glyph_index, FT_LOAD_RENDER );
    FT_Render_Glyph( ftFace->glyph, ft_render_mode_normal );
    
    img->pasteFTBitmap( &slot->bitmap, borderWidth, borderWidth, fg, 0 );	 
#endif
    
    return chip;
}
