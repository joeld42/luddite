#include <stdarg.h>

#include <string>
#include <vector>
#include <algorithm>

#include <png.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "image.h"
#include "chip.h"
#include "binpack.h"

#define ALL_PUNCTUATION "!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~"


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
        bool isPattern = false;
        
        if ( (!strcmp( ch, "A-Z" )) || (!strcmp( ch, "A-z" )) )
        {
            for ( char c='A'; c <= 'Z'; c++)
            {
                if (charset.find(c) == std::string::npos)
                {
                    charset.push_back( c );
                }
            }
            isPattern = true;
        }
        
        if ( (!strcmp( ch, "a-z" )) || (!strcmp( ch, "A-z" )) )
        {
            for ( char c='a'; c <= 'z'; c++)
            {
                if (charset.find(c) == std::string::npos)
                {
                    charset.push_back( c );
                }
            }
            isPattern = true;
        }
        
        if (!strcmp( ch, "0-9" ) )
        {
            for ( char c='0'; c <= '9'; c++)
            {
                if (charset.find(c) == std::string::npos)
                {
                    charset.push_back( c );
                }
            }
            isPattern = true;
        }
        
        if (!strcmp( ch, "punct" ))
        {
            for (const char *c = ALL_PUNCTUATION; *c; c++)
            {
                if (charset.find( *c) == std::string::npos)
                {
                    charset.push_back( *c );
                }
            }
            isPattern = true;
        }
        
        if (!strcmp( ch, "comma" ))
        {
            if (charset.find(',') == std::string::npos)
            {
                charset.push_back( ',' );
            }
            isPattern = true;
        }    
        
        // all printable chars
        if (!strcmp( ch, "all" ))
        {
            for (int c=0; c < 127; c++)
            {
                if (isprint(c) && (charset.find(c) == std::string::npos))
                {
                    charset.push_back( c );
                }
            }
            isPattern = true;
        }

        // If this wasn't a pattern, add the characters directly
        if (!isPattern)
        {
            
            for (char *c = ch; *c; c++)
            {
                if (charset.find( *c) == std::string::npos)
                {
                    charset.push_back( *c );
                }
            }
        }
        
        // next pattern
        ch = strtok( NULL, "," );
    }
    
    // sort the charset just for tidyness
    std::sort( charset.begin(), charset.end() );
    
    printf("Charset is %s\n", charset.c_str() );
    
    free(patternStr);
    
    return charset;
}

// ===========================================================================
int main( int argc, char *argv[] )
{
    FT_Library ftLibrary;
    FT_Init_FreeType( &ftLibrary );
    FT_Face ftFace;
    
    // appearance
    int borderWidth = 0;
    
    //various outputs
    std::string outFontImg;
    std::string outFinfoFile;
    std::string outCFile;
    std::string outXmlFile;

    // Current charset
    std::string charSet = makeCharset( "A-Z" );
    
    // List of 'chips' to pack
    std::vector<Chip*> m_chipsToPack;
    
    int ndx = 1;
    while (ndx < argc)
    {
        std::string arg = argv[ndx++];
        if ((arg=="--font")||(arg=="-f"))
        {
            // make sure there is an arg available
            if (ndx+1 > argc)
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

            int ft_error = FT_New_Face( ftLibrary, fontfile.c_str(), 0, &ftFace );     
            if (ft_error) {
                errorMsg( "Can't load font %s, [FT_Error 0x%02X]",
                         fontfile.c_str(), ft_error );
            }
            
            if (!ftFace->charmap) 
            {
                if (ftFace->num_charmaps==0) 
                {
                    errorMsg("Font '%s' has no charmaps [FT_Error 0x%02X]\n",
                             fontfile.c_str(), ft_error );
                } 
                else
                {
                    // use the first available charmap
                    FT_Set_Charmap( ftFace, ftFace->charmaps[0] );	     
                }
            }
            
            // Font is loaded, set size
            FT_Set_Pixel_Sizes( ftFace, 0, pxlSize );
            
            // Now rasterize and add the current charset to the list 
            // to pack
            for (std::string::iterator ch = charSet.begin();
                 ch != charSet.end(); ch++)
            {
                Chip *glyphChip = Chip::makeGlyph( &ftLibrary, ftFace, 
                                                  *ch, borderWidth );
                m_chipsToPack.push_back( glyphChip );
            }
        }
        else if ((arg=="-o")||(arg=="--out"))
        {
            // make sure there is an arg available
            if (ndx+1 > argc)
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
            if (ndx+1 > argc)
            {
                errorMsg( "--charset requires argument." );
            }
            
            std::string pattern = argv[ndx++];
            charSet = makeCharset( pattern );
        }
        else if ((arg=="--border") || (arg=="-b"))
        {
            if (ndx+1 > argc)
            {
                errorMsg( "--border requires argument." );
            }
            borderWidth = atoi( argv[ndx++] );
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
    
    // pack the chips
    printf("Will pack %d glyphs\n", m_chipsToPack.size() );
    FpImage *outImg = packChips( m_chipsToPack ); // porkChops?
    

    
    // save chip 0 for testing
    m_chipsToPack[0]->m_img->writePng( "test.png" );
    
    // output image
    //Image fontImg( 512, 512 );
    
    printf("Saving font image %s...\n", outFontImg.c_str() );
    //fontImg.writePng( outFontImg.c_str() );
    
}
