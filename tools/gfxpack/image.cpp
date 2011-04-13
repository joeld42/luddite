#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "image.h"

// This is kind of a mess because it was converted from earlier 
// C code that was really a mess.
FpImage::FpImage()
{
    w = 0;
    h = 0;
    buf = NULL;    
}

FpImage::FpImage( int width, int height, unsigned long color )
{
    w = width;
    h = height;    
    buf = (unsigned char *)malloc( w*h*3 );
    clear( color );    
}

FpImage::~FpImage()
{
    free(buf);
}


FpImage::FpImage( const FpImage &other )
{
    buf = (unsigned char *)malloc(other.w*other.h*3 );
    w = other.w;
    h = other.h;    
    paste( other, 0, 0 );
}

FpImage &FpImage::operator=( const FpImage &other ) 
{
    buf = (unsigned char *)malloc(other.w*other.h*3 );
    w = other.w;
    h = other.h;    
    paste( other, 0, 0 );
    
    return *this;    
}


void FpImage::clear( unsigned long color )
{
    unsigned char r,g,b;    
    unsigned char *p = buf;
    
    r = (unsigned char)((color >> 16) & 0xff);
    g = (unsigned char)((color >> 8) & 0xff);
    b = (unsigned char)(color & 0xff);
    
    for (int i=0; i < w*h; i++) {
		*(p++) = r;
		*(p++) = g;
		*(p++) = b;
    } 
}

// swizzle color channel from BGR to RGB or back
void FpImage::swapBR()
{
    unsigned char r,g,b;    
    unsigned char *p = buf;       
    
    for (int i=0; i < w*h; i++) {
        
		b = *p;
		g = *(p+1);
		r = *(p+2);
        
		*(p++) = r;
		*(p++) = g;
		*(p++) = b;
    }
}

void FpImage::paste( const FpImage &other, int pos_x, int pos_y )
{
    unsigned char *dest_p, *src_p;
    int x, y;
    
    for (int i =0; i < other.w; i++) {
        for (int j=0; j < other.h; j++) {
            x = pos_x + i;
            y = pos_y + j;
            if ((x >= 0) && (x <= w) &&
                (y >= 0) && (y <= h) ) {
                dest_p = buf + (x + (y*w))*3;
                src_p  = other.buf + (i + (j*other.w))*3;
                
                *(dest_p++) = *(src_p++);
                *(dest_p++) = *(src_p++);
                *(dest_p++) = *(src_p++);
            }
        }	
    }
}

void FpImage::pasteFTBitmap( FT_Bitmap *glyph_bmp, int x, int y,
                            unsigned long color, int mono )
{
    unsigned char *dest_p, *src_p;
    int ii, jj;
    float t;
    
	
	//    printf("x %d y %d rows %d cols %d\n",
	//	   x, y, glyph_bmp->rows, glyph_bmp->width );
    
	
    for (int j = 0; j < glyph_bmp->rows; j++) {
		for (int i=0; i < glyph_bmp->width; i++) {
			
			ii = x + i;
			jj = y + j;
			
			if ((ii>=0) && (ii<w)&&(jj>=0)&&(jj<h)) {
				dest_p = buf + (ii + (jj * w))*3;
				
				if (mono) {
					src_p = glyph_bmp->buffer +  (i/8) + (j*glyph_bmp->pitch);
					if (*src_p & (0x1 << (7-(i%8)))) {
						t = 1.0;			
					} else {
						t = 0.0;			
					}		    
				}
				else {
					src_p = glyph_bmp->buffer + i + (j*glyph_bmp->pitch);
					t = (float)(*src_p) / 255.0f;		
				}
				
				
				dest_p[0] = (char)( (t * ((color >> 16) & 0xff)) +  
                                   ((1.0-t) * dest_p[0]) );
				dest_p[1] = (char)( (t * ((color >> 8 ) & 0xff)) +  
                                   ((1.0-t) * dest_p[1]) );
				dest_p[2] = (char)( (t * (color & 0xff)) +  
                                   ((1.0-t) * dest_p[2]) );
			}
			
		}	
    }
}


void FpImage::thicken( unsigned long bgcolor ) 
{
    unsigned char *img2 = (unsigned char *)malloc( w*h*3 ),
	*img_p, *img2_p;
    int x, y;    
    unsigned char r, g, b;
    
    unsigned char bg_r,bg_g,bg_b;    
    bg_r = (unsigned char)((bgcolor >> 16) & 0xff);
    bg_g = (unsigned char)((bgcolor >> 8) & 0xff);
    bg_b = (unsigned char)(bgcolor & 0xff);
    
    
    for (int j=0; j < h; j++) {
        for (int i=0; i < w; i++) {
            
            r = bg_r; g = bg_g; b = bg_b;	    
            for (int jj = -1; jj <= 1; jj++) {
                for (int ii=-1; ii <=1; ii++ ) {
                    if ((ii==0)||(jj==0)) {
                        
                        x = i + ii;
                        y = j + jj;
                        if (( x>=0) && (x<w)&&(y>=0)&&(y<h)) {
                            img_p = buf + ((y * w) + x)*3;
                            r = img_p[0];
                            g = img_p[1];
                            b = img_p[2];
                            
                            
                            if ((r!=bg_r)||(g!=bg_g)||(b!=bg_b)) {
                                break;
                            }
                        }
                        
                    }
                }
                
                if ((r!=bg_r)||(g!=bg_g)||(b!=bg_b))break;
            }
            
            img2_p = img2 + ((j*w) + i)*3;
            img2_p[0] = r;
            img2_p[1] = g;
            img2_p[2] = b;
            
        }	
    }
    
    memcpy( buf, img2, w*h*3 );
    free(img2);
}

void FpImage::autoCrop( unsigned long bgcolor )
{
    unsigned char *shrunk_img, *img_p, 
	*img = buf;
    int x1, y1, x2, y2; 
    
    unsigned char bg_r,bg_g,bg_b;    
    bg_r = (unsigned char)((bgcolor >> 16) & 0xff);
    bg_g = (unsigned char)((bgcolor >> 8) & 0xff);
    bg_b = (unsigned char)(bgcolor & 0xff);
    
    // find xmin
    int bail=0;    
    x1 = 0;
    while ((!bail)&&(x1 < w)) {
        for (int j=0; j < h; j++) {
            img_p = img + (x1 + (j*w))*3;
            if ( (*img_p != bg_r) ||
                (*(img_p+1) != bg_g ) ||
                (*(img_p+2) != bg_b )) {
                bail = 1;		
                break;		
            }
            
        }	
        if (!bail) x1++;
    }
    if (!bail) return;
    
    // find xmax
    bail=0;    
    x2 = w-1;
    while ((!bail)&&(x2>=x1)) {
        for (int j=0; j < h; j++) {
            img_p = img + (x2 + (j*w))*3;
            if ( (*img_p != bg_r) ||
                (*(img_p+1) != bg_g ) ||
                (*(img_p+2) != bg_b )) {
                bail = 1;		
                break;		
            }
        }	
        if (!bail) x2--;
    }
    if (!bail) return;
    
    
    // find ymin
    bail=0;    
    y1 = 0;
    while ((!bail)&&(y1 < h)) {
        for (int j=0; j < w; j++) {
            img_p = img + (j + (y1*w))*3;
            if ( (*img_p != bg_r) ||
                (*(img_p+1) != bg_g ) ||
                (*(img_p+2) != bg_b )) {
                bail = 1;		
                break;		
            }
            
        }	
        if (!bail) y1++;
    }
    if (!bail) return;
    
    // find ymax
    bail=0;    
    y2 = w-1;
    while ((!bail)&&(y2>=y1)) {
        
        for (int j=0; j < w; j++) {
            img_p = img + (j + (y2*w))*3;
            if ( (*img_p != bg_r) ||
                (*(img_p+1) != bg_g ) ||
                (*(img_p+2) != bg_b )) {
                bail = 1;		
                break;		
            }
        }	
        if (!bail) y2--;
    }
    if (!bail) return;
    
    int new_w, new_h, x, y;
    unsigned char *dest_p, *src_p;    
    new_w = (x2-x1)+1;
    new_h = (y2-y1)+1;
    
    shrunk_img = (unsigned char *)malloc( new_w * new_h * 3 );
    for (int i =0; i < new_w; i++) {
        for (int j=0; j < new_h; j++) {
            x = x1 + i;
            y = y1 + j;
            if ((x >= 0) && (x <= new_w) &&
                (y >= 0) && (y <= new_h) ) {
                dest_p = shrunk_img + (i + (j*new_w))*3;
                src_p  = img + (x + (y*w))*3;
                
                *(dest_p++) = *(src_p++);
                *(dest_p++) = *(src_p++);
                *(dest_p++) = *(src_p++);
            }
        }	
    }
    // done
    free (buf);
    buf = shrunk_img;
    w = new_w;
    h = new_h;    
}



// unit test for FpImage
bool FpImage::selfTest()
{
    int i,j,k;
    
    // create and clear an image
    printf("FpImage::selfTest().....Create and clear an image\n");    
    unsigned char *p;    
    FpImage img( 100, 100, 0xa0a0a0 );
    p = img.buf;
    
    for (j=0; j < img.h; j++) {
        for (i=0; i < img.w; i++) {
            assert(0xa0==(*p)); p++;
            assert(0xa0==(*p)); p++;
            assert(0xa0==(*p)); p++;
        }	
    }
    
    // copy an image
    printf("FpImage::selfTest().....Copy constructor\n");
    FpImage img2 = FpImage( img );    
    p = img2.buf;
    
    for ( j=0; j < img2.h; j++) {
        for (int i=0; i < img2.w; i++) {
            assert(0xa0==(*p)); p++;
            assert(0xa0==(*p)); p++;
            assert(0xa0==(*p)); p++;
        }	
    }
    
    
    // thicken
    printf("FpImage::selfTest().....Thicken\n");
    FpImage img3( 20, 20, 0xff00ff );
    
    img3.buf[ (10 + (10*20))*3 + 0 ] = 0xA0; // seed pixel
    img3.buf[ (10 + (10*20))*3 + 1 ] = 0xA0;
    img3.buf[ (10 + (10*20))*3 + 2 ] = 0xA0;
    
    int expectedFgPixels[] = {
        1,5,13,25,41,61,85,113,145,181,219,
        255,287,315,339,359,375,387,395,399,400,
        400, 400 };    
    
    for (k=0; k < sizeof(expectedFgPixels)/sizeof(int); k++) {
        int fg_count = 0;
        int bg_count = 0;
        
        p = img3.buf;	
        for (int j=0; j < img3.h; j++) {
            for (int i=0; i < img3.w; i++) {
                if (*p==0xff) {
                    bg_count++;
                    assert(0xff==(*p)); p++;
                    assert(0x00==(*p)); p++;
                    assert(0xff==(*p)); p++;
                } else if (*p==0xa0) {
                    fg_count++;
                    assert(0xa0==(*p)); p++;
                    assert(0xa0==(*p)); p++;
                    assert(0xa0==(*p)); p++;
                } else {
                    // some other color
                    assert( 0 );		    
                }
            }
        }
        
        // make sure the count is what we expect
        assert( fg_count  + bg_count == 400 );
        assert( expectedFgPixels[k] == fg_count );	
        
        img3.thicken( 0xff00ff );	
    }
    
    
    // auto crop
    printf("FpImage::selfTest().....AutoCrop\n");
    FpImage img_src( 20, 20, 0xff00ff );
    FpImage img_cropped;
    
    img_src.buf[ (10 + (10*20))*3 + 0 ] = 0xA0; // seed pixel
    img_src.buf[ (10 + (10*20))*3 + 1 ] = 0xA0;
    img_src.buf[ (10 + (10*20))*3 + 2 ] = 0xA0;
    
    int expectedSize = 1;    
    for ( k=0; k < 13; k++) {
        img_cropped = FpImage( img_src );
        
        img_cropped.autoCrop( 0xff00ff );
        assert( img_cropped.w == expectedSize );
        assert( img_cropped.h == expectedSize );
        expectedSize += 2;
        if (expectedSize > 20) expectedSize=20;	
        
        img_src.thicken( 0xff00ff );
    }
    
    
    return true;    
}




