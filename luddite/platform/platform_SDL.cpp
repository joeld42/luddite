// based somewhat on PNGLoader.c from Alexander Okafor
//
#include <luddite/core/debug.h>

#include <png.h>
#include "platform.h"

using namespace luddite;

#define PNG_HEADER_SIZE (8)

void png_read_premultiply_alpha(png_structp png_ptr, png_row_infop row_info, png_bytep data);

int pow2(int n)
{
	int x = 1;
	while( x < n)
	{
		x <<= 1;
	}
	return x;
}


bool platform_loadTexture( const char *filename, 
                           luddite::TextureGL *texture,
                           unsigned char **out_pixelData )
{    
    // right now just load PNG
    FILE *fpPng = fopen( filename, "rb");
    
    // TODO: return 'MISSING' texture on error
    if (!fpPng) return false;
    
    GLubyte PNG_header[ PNG_HEADER_SIZE ];
    fread (PNG_header, 1, PNG_HEADER_SIZE, fpPng );
    if (png_sig_cmp( PNG_header, 0, PNG_HEADER_SIZE ) != 0)
    {
        DBG::warn( "%s is not a PNG file\n", filename );
        return false;        
    }

    png_structp PNG_reader = png_create_read_struct(
        PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (PNG_reader == NULL)
    {
        DBG::warn("Can't start reading PNG file %s\n", filename );
        fclose(fpPng);
        return false;        
    }
	
    png_infop PNG_info = png_create_info_struct(PNG_reader);
    if (PNG_info == NULL)
    {
        DBG::warn("Can't get info for PNG file %s\n", filename);
		
        png_destroy_read_struct(&PNG_reader, NULL, NULL);
        fclose(fpPng);
        return false;        
    }
	
    png_infop PNG_end_info = png_create_info_struct(PNG_reader);
    if (PNG_end_info == NULL)
    {
        DBG::warn("Can't get end info for PNG file %s\n", filename);
		
        png_destroy_read_struct(&PNG_reader, &PNG_info, NULL);
        fclose(fpPng);
        return false;        
    }
    
    if (setjmp(png_jmpbuf(PNG_reader)))
    {
        DBG::warn("Can't load PNG file %s\n", filename);
		
        png_destroy_read_struct(&PNG_reader, &PNG_info, &PNG_end_info);
        fclose(fpPng);
        return false;        
    }
    
    png_init_io(PNG_reader, fpPng);
    png_set_sig_bytes(PNG_reader, PNG_HEADER_SIZE);
    
    png_read_info(PNG_reader, PNG_info);
	
	// we have to do a custom transformation to premultiply the alpha of the image
    png_set_read_user_transform_fn(PNG_reader, png_read_premultiply_alpha);
    
	png_uint_32 width, height;
    int bit_depth, color_type;
    
	png_get_IHDR(PNG_reader, PNG_info, &width, &height, &bit_depth, &color_type,NULL, NULL, NULL);
	
    if (color_type == PNG_COLOR_TYPE_PALETTE)
    {
        png_set_palette_to_rgb(PNG_reader);
    }
	
    if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8) 
    {
        png_set_gray_1_2_4_to_8(PNG_reader);
    }
    
    if (color_type == PNG_COLOR_TYPE_GRAY ||
        color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
    {
        png_set_gray_to_rgb(PNG_reader);
    }
	
    if (png_get_valid(PNG_reader, PNG_info, PNG_INFO_tRNS))
    {
        png_set_tRNS_to_alpha(PNG_reader);
    }
    else
    {
        png_set_filler(PNG_reader, 0xff, PNG_FILLER_AFTER);
    }
    

    if (bit_depth == 16)
    {
        png_set_strip_16(PNG_reader);
    }
	
    png_read_update_info(PNG_reader, PNG_info);
    
	png_uint_32 widthPow2, heightPow2;
	widthPow2 = pow2(width);
	heightPow2 = pow2(height);
	
    // FIXME: use an allocator here
    png_byte* PNG_image_buffer = (png_byte*)malloc(4 * widthPow2 * heightPow2);
	memset(PNG_image_buffer,0,4*widthPow2*heightPow2); // clear image buffer
    png_byte** PNG_rows = (png_byte**)malloc(height * sizeof(png_byte*));


	png_uint_32 rowBytes = widthPow2*4;

	// load the image from the bottom up
	/*
	 image texture in mem looks like:
	 --------
	 |       |
	 |xxxx	 |
	 |xxxx	 |
	 --------
	 where 'x's represent actual image data and the lines are the image buffer.
	 so the image is aligned at the (0,0) texel coordinate of the image buffer.
	 */
	unsigned int row;
    for (row = 0; row < height; ++row)
    {
        PNG_rows[height-1-row] = PNG_image_buffer + (row * rowBytes);
    }
    png_read_image(PNG_reader, PNG_rows);
	
    free(PNG_rows);
   
    png_destroy_read_struct(&PNG_reader, &PNG_info, &PNG_end_info);
    fclose(fpPng);
    
    // fill in the texture info
    texture->m_width = widthPow2;
    texture->m_height = heightPow2;

    texture->m_origWidth = width;
    texture->m_origHeight = height;
    
    // return the texture data
    *out_pixelData = PNG_image_buffer;    
    
}

// pre-multiply the color channels by the alpha
void png_read_premultiply_alpha(png_structp png_ptr, png_row_infop row_info, png_bytep data)
{
	png_bytep row_pointer = data;
	if (png_ptr == NULL)return;
	
	/* Contents of row_info:
	 *  png_uint_32 width      width of row
	 *  png_uint_32 rowbytes   number of bytes in row
	 *  png_byte color_type    color type of pixels
	 *  png_byte bit_depth     bit depth of samples
	 *  png_byte channels      number of channels (1-4)
	 *  png_byte pixel_depth   bits per pixel (depth*channels)
	 */
	if (row_info->bit_depth == 8) // only supporting 8-bit depth png's right now.
	{	
		png_uint_32 rowBytes;
		rowBytes = row_info->rowbytes;
		png_byte *pixel = row_pointer;
		png_byte *stop = row_pointer + rowBytes;
		//int pos =0;
		while (pixel != stop)
		{
			//png_byte *pixel = row_pointer + (pos*row_info->channels);
			float alphaMultiply = pixel[3] / 255.0;
			pixel[0] = (png_byte)(pixel[0] * alphaMultiply);
			pixel[1] = (png_byte)(pixel[1] * alphaMultiply);
			pixel[2] = (png_byte)(pixel[2] * alphaMultiply);
			
			pixel += row_info->channels;
		}
	}
}




