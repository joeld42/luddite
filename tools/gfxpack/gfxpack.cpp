#include <stdarg.h>

#include <string>
#include <vector>

#include <png.h>

#include <ft2build.h>
#include FT_FREETYPE_H

// ===========================================================================
void doErrorMsg( const char *msg )
{
    puts( "ERROR: " );
    puts( msg );
    if (msg[strlen(msg)-1] != '\n') puts( "\n" );
    
    exit(1);
}

void errorMsg( const char *msg, ...)
{
	char *buff;
	int bsz, sz;
    
	va_list args;
	va_start(args, msg);
	
	// make space for the new string, try something
    // reasonable first
	sz = strlen( msg ) + 100;
    
	// format the string
	while (1) {
		buff = (char*)malloc( sz );
		bsz = vsnprintf( buff, sz, msg, args );
        
		if (bsz >= 0) {
			doErrorMsg( buff );
			free(buff);
			break;
		}
        
		// try again with twice as big until it fits
		free( buff );
		sz <<= 1;
	}
    
	va_end(args);	
}



// ===========================================================================
// super-simple RGBA8 image class
class Image
{
public:
    Image( int w, int h );
    ~Image();
    
    void writePng( const char *filename );
    
protected:
    int m_width, m_height;
    unsigned char *m_data;
};

Image::Image( int w, int h ) :
    m_width(w),
    m_height(h)
{
    m_data = (unsigned char *)calloc( sizeof(unsigned char), 
                                    m_width*m_height*4 );
}

Image::~Image()
{
    free(m_data);
}

void Image::writePng( const char *filename )
{
    FILE *fp = fopen( filename, "wb" );
    if (!fp)
    {
        printf( "Error writing cache file %s\n", filename );
        return;
    }
    
    png_structp png_ptr = png_create_write_struct( PNG_LIBPNG_VER_STRING,
                                                  NULL, NULL, NULL );
    
    if (!png_ptr)
    {
        errorMsg( "error creating png write struct\n" );
    }   
    
    png_infop info_ptr = png_create_info_struct( png_ptr );
    if (!info_ptr)
    {
        png_destroy_write_struct( &png_ptr, (png_infopp)NULL );
        errorMsg( "error creating png info struct\n" );
    }
    
    // init IO and compression
    png_init_io( png_ptr, fp );
    png_set_compression_level( png_ptr, Z_BEST_COMPRESSION );	
    
    // set content header
    png_set_IHDR( png_ptr, info_ptr, m_width, m_height,
                 8, PNG_COLOR_TYPE_RGB,
                 PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_DEFAULT,
                 PNG_FILTER_TYPE_DEFAULT );
    
    // write header info
    png_write_info(png_ptr, info_ptr);	
    
    // write the thing
    png_byte *row_pointers[m_height];
    for (int i=0; i < m_height; i++)
    {
        // flip 
        //row_pointers[i] = &(data[(1023-i) * sz * 3]);
        
        row_pointers[i] = &(m_data[i * m_width * 4]);
    }
    png_write_image( png_ptr, row_pointers );
    
    png_write_end( png_ptr, info_ptr );
    png_destroy_write_struct( &png_ptr, &info_ptr );
    
    fclose( fp );
    
    printf( "Wrote image %s\n", filename );
}
    

// ===========================================================================
struct Glyph
{
    unsigned char m_char;
    int m_xpos, m_ypos, m_baseline;
};

// ===========================================================================
// Takes a pattern string like A-Z,a-z,0-9,1234,ASdsda){],comma and returns
// the string of characters represented.
//
//   Supported pattern strings:
//      A-Z   - uppercase alphabet
//      a-z   - lowercase alphabet
//      A-z   - upper and lowercase alphabets
//      0-9   - digits. Doesn't support a range
//      punct - standard punctuation
//      all   - all printable characters.
//      comma - a comma (since this is the separator)
//
//      anything else - those characters
//
// Right now only simple ascii (not extended ascii) characters are supported
std::string makeCharset( std::string pattern )
{
    char *patternStr = strdup( pattern.c_str() );
    
    std::string charset;
    
    char *ch = strtok( patternStr, "," );
    while (ch)
    {
        printf( "PATTERN: %s\n", ch );
        ch = strtok( NULL, "," );
    }
    
    free(patternStr);
}

// ===========================================================================
int main( int argc, char *argv[] )
{
    FT_Library ftLibrary;
    FT_Init_FreeType( &ftLibrary );
    
    //various outputs
    std::string outFontImg;
    std::string outFinfoFile;
    std::string outCFile;
    std::string outXmlFile;

    // Current charset
    std::string charSet = makeCharset( "A-Z" );
    
    int ndx = 1;
    while (ndx < argc)
    {
        std::string arg = argv[ndx++];
        if ((arg=="--font")||(arg=="-f"))
        {
            // make sure there is an arg available
            if (ndx+1 >= argc)
            {
                errorMsg( "--font requires argument." );
            }
            
            int pxlSize = 12;
            std::string fontArg = argv[ndx++];
            
            std::string fontfile;

            // Did specify font size, too?
            size_t separator_pos = fontArg.rfind( ':' );
            if (separator_pos != std::string::npos)
            {
                fontfile = fontArg.substr( 0, separator_pos );
                std::string sz = fontArg.substr( separator_pos+1 );
                printf("sz is %s\n", sz.c_str() );
                
                pxlSize = atoi( sz.c_str() );
            }
            else
            {
                fontfile = fontArg;                
            }
            

            printf( "TODO: set font %s size %d\n",
                    fontfile.c_str(),
                    pxlSize );            
            
        }
        else if ((arg=="-o")||(arg=="--out"))
        {
            // make sure there is an arg available
            if (ndx+1 >= argc)
            {
                errorMsg( "--out requires argument." );
            }
            
            std::string outFile = argv[ndx++];
            
            // Look at file type
            size_t ext_pos = outFile.rfind( '.' );
            if (ext_pos == std::string::npos)
            {
                errorMsg( "Couldn't determing extension for output %s\n", 
                         outFile.c_str() );
            }
            else
            {
                // what kind of output is it?
                std::string ext = outFile.substr( ext_pos+1 );
                if (ext=="png")
                {
                    outFontImg = outFile;
                }
                else if (ext=="finfo")
                {
                    outFinfoFile = outFile;
                }
                else if ((ext=="cpp")||(ext=="c"))
                {
                    outCFile = outFile;
                }
                else if (ext=="xml")
                {
                    outXmlFile = outFile;
                }
                else
                {
                    errorMsg( "Unknown output type '%s' for --out %s",
                             ext.c_str(),
                             outFile.c_str() );
                }
            }
            
            printf( "TODO: output %s\n", outFile.c_str() );
        }
        else if ((arg=="--charset")||(arg=="-c"))
        {
            // make sure there is an arg available
            if (ndx+1 >= argc)
            {
                errorMsg( "--charset requires argument." );
            }
            
            std::string pattern = argv[ndx++];
            charSet = makeCharset( pattern );
        }
        else
        {
            errorMsg( "Unknown argument %s\n", argv[ndx] );
        }
    }
    
    // Check for required args
    
    // out image
    if (outFontImg.empty())
    {
        errorMsg( "No output image specified (use \"--out image.png\")" );
    }
    
    printf( "TODO: pack font\n" );
    
    // output image
    Image fontImg( 512, 512 );
    
    printf("Saving font image %s...\n", outFontImg.c_str() );
    fontImg.writePng( outFontImg.c_str() );
    
}
