#ifndef FP_IMAGE_H
#define FP_IMAGE_H

#include <ft2build.h>
#include FT_FREETYPE_H

// image class salvaged from fontpack

// very simplistic image class, this is not a general purpose
// class but very specific to what fontpack needs to do.

class FpImage
{
public:
    FpImage();
    FpImage( int width, int height, unsigned long color );
    FpImage( const FpImage &other );
    virtual ~FpImage();

    FpImage &operator=( const FpImage &other );    

    void clear( unsigned long color );
    //void draw_FT_Bitmap( FT_Bitmap *glyph_bmp, int x, int y,
    //			 unsigned long color, int mono );

    // save as a png
    void writePng( const char *filename );
    
    // "thicken"s the image by setting any pixel adjacent to a fg
    // to a fg color. Assumes image is 2-color (fg, bg) at this stage
    void thicken( unsigned long bgcolor );

    // crops as small as possible, discarding only bgcolor
    void autoCrop( unsigned long bgcolor );

	// paste onto this image
    void paste( const FpImage &other, int pos_x, int pos_y );
	
	void swapBR(); // swizzle color channel from BGR to RGB or back

	// special function to convert from a freetype FT_Bitmap
	void pasteFTBitmap( FT_Bitmap *glyph_bmp, int x, int y,
								 unsigned long color, int mono );

    //static bool selfTest();  
	
	int getWidth() const { return w; }
	int getHeight() const { return h; }

	unsigned char *getData() { return buf; };

protected:    
    int w, h;
    unsigned char *buf;
};


#endif

