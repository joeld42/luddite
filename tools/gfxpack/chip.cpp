//
//  chip.cpp
//  luddite
//
//  Created by Joel Davis on 4/13/11.
//  Copyright 2011 Joel Davis. All rights reserved.
//

#include "chip.h"
#include "image.h"

Chip *Chip::makeGlyph(  FT_Library *ft, FT_Face ftFace, 
                      int ch, int borderWidth )
{
    // TODO: un-hardcode this
    unsigned long borderColor = 0xffffaa33;
    unsigned long bgColor = 0x00000000;
    unsigned long fgColor = 0xFFaa4488;
    
    Chip *chip = new Chip();
    
    int glyph_index = FT_Get_Char_Index( ftFace, ch );	
	FT_GlyphSlot slot = ftFace->glyph;
    
	// draw the glyph without antialiasing to get the char size and
	// to seed the border
	FT_Load_Glyph( ftFace, glyph_index, FT_LOAD_MONOCHROME );
	FT_Render_Glyph( ftFace->glyph, ft_render_mode_mono );
    
    // packing info
    chip->m_xpos = 0;
    chip->m_ypos = 0;
    chip->m_width  = slot->bitmap.width;
    chip->m_height = slot->bitmap.rows;
    
    // font info
    chip->m_char = ch;
    chip->m_baseline = slot->bitmap_top;
    
    // make space for border
	chip->m_width += borderWidth*2;
	chip->m_height += borderWidth*2;	    	
    
    // TODO: background color
    chip->m_img = new FpImage( chip->m_width, chip->m_height, bgColor );
	
    if (borderWidth) {
        
        printf("border width %d\n", borderWidth );
        
        chip->m_img->pasteFTBitmap( &slot->bitmap, borderWidth, borderWidth, 
                                   borderColor, 1 );
        
        for (int b=0; b < borderWidth; b++) 
        {		
            chip->m_img->thicken( bgColor );
        }
    }
	
	// render the glyph again, antialiased, and put it on top of the border
    FT_Load_Glyph( ftFace, glyph_index, FT_LOAD_RENDER );
    FT_Render_Glyph( ftFace->glyph, ft_render_mode_normal );
    
    chip->m_img->pasteFTBitmap( &slot->bitmap, 
                               borderWidth, borderWidth, 
                               fgColor, 0 );	 
    
    printf("makeGlyph %c\n", ch );

    return chip;
}
