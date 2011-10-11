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
                        int ch, int borderWidth,
                        unsigned long fgColor,
                        unsigned long bgColor,
                        unsigned long borderColor )
{    
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
    chip->m_pxlsize = 0; // don't know it here
    chip->m_width  = slot->bitmap.width;
    chip->m_height = slot->bitmap.rows;
    chip->m_role = ChipRole_GLYPH;
    
    // font info
    chip->m_char = ch;
    chip->m_baseline = slot->bitmap_top;
    
    // make space for border
	chip->m_width += borderWidth*2;
	chip->m_height += borderWidth*2;	    	
    
    chip->m_img = new FpImage( chip->m_width, chip->m_height, bgColor );
	
    if (borderWidth) {
        
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
    
    return chip;
}


// TODO: draw char code in box
Chip *Chip::makeExtra( int ch, int w, int h, int pxlsize,
                       unsigned long fgColor,
                       unsigned long bgColor )
{
    // add some extras
    Chip *extraChip = new Chip();
    
    // packing info
    extraChip->m_xpos = 0;
    extraChip->m_ypos = 0;
    extraChip->m_pxlsize = 8;
    extraChip->m_width  = 10;
    extraChip->m_height = 10;
    extraChip->m_role = ChipRole_GLYPH;
    
    // font info
    extraChip->m_char = ch;
    extraChip->m_baseline = 0;
    
    // make image for the chip
    extraChip->m_img = new FpImage( 
         extraChip->m_width, extraChip->m_height, bgColor );
    
    // draw border (CBB: move into image)
    for (int ii=0; ii < extraChip->m_img->getWidth(); ii++)
    {
        extraChip->m_img->setPixel( ii, 0, fgColor );
        extraChip->m_img->setPixel( ii, extraChip->m_img->getHeight()-1, fgColor );
    }
    
    for (int jj=1; jj < extraChip->m_img->getHeight()-1; jj++)
    {
        extraChip->m_img->setPixel( 0, jj, fgColor );
        extraChip->m_img->setPixel( extraChip->m_img->getWidth()-1, jj, fgColor );
    }

    
    return extraChip;
}

Chip *Chip::makeSprite( const char *spriteName, FpImage *srcImg )
{
    Chip *spriteChip = new Chip();
    
    // packing info
    spriteChip->m_xpos = 0;
    spriteChip->m_ypos = 0;
    spriteChip->m_pxlsize = 8;
    spriteChip->m_width  = srcImg->getWidth();
    spriteChip->m_height = srcImg->getHeight();
    spriteChip->m_role = ChipRole_SPRITE;
    spriteChip->m_spriteName = strdup( spriteName );
    
    // clip ext
    char *ch = strrchr( spriteChip->m_spriteName, '.');
    if (ch) *ch = '\0';
    
    spriteChip->m_img = srcImg;
    
    return spriteChip;
    
}
