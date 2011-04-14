#include <stdarg.h>
#include <time.h>

#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <iostream>

#include <png.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "image.h"
#include "chip.h"
#include "binpack.h"

#define ALL_PUNCTUATION "!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~"


// ==========================================================================
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
unsigned long parseColor( std::string color )
{
    if (color.empty()) return 0xff000000;
    int r, g, b;

    char *colorErr = "Invalid color '%s', expected color of the form 'R,G,B', '#RRGGBB' or '#RGB'";    
    
    // 'web-style' hex value
    if (color[0]=='#')
    {
        if (color.size()==4)
        {
            // #rgb 4-bpc
            int num = strtol( color.c_str()+1, NULL, 16 );
            r = (num & 0xf00) >> 8;
            g = (num & 0x0f0) >> 4;
            (b = num & 0x00f);
            
            // double them up, i.e. #fed -> #ffeedd
            r = (r << 4) | r;
            g = (g << 4) | g;
            b = (b << 4) | b;            
        }
        else if (color.size()==7)
        {
            // rgb 8-bpc
            int num = strtol( color.c_str()+1, NULL, 16 );
            r = (num & 0xff0000) >> 16;
            g = (num & 0x00ff00) >> 8;
            b = (num & 0x0000ff);
        }
        else
        {
            errorMsg( colorErr, color.c_str() );            
        }        
    }
    else
    {
        // "tuple" style color
        char *colorStr = strdup( color.c_str() );

        // replace everything that's not a hex digit with spaces
        for (char *ch=colorStr; *ch; ++ch )
        {
            if (!( ((*ch>='0') && (*ch<='9')) ||
                   ((*ch>='A') && (*ch<='F')) ||
                   ((*ch>='a') && (*ch<='f')) ) )
            {
                // not a digit
                *ch = ' ';                
            }            
        }

        // now use strtok to split the string
        r = g = b = 0;
        char *ch = strtok( colorStr, " " );
        int ndx =0;
        while (ch)
        {
            int n = atoi(ch);
            switch (ndx)            
            {
            case 0:
                r = n;
                break;
            case 1:
                g = n;
                break;
            case 2:
                b = n;
                break;
            default:
                errorMsg( colorErr, color.c_str() );
                break;                
            }            
            
            ndx++;            
            ch = strtok( NULL, " " );            
        }
        

        free(colorStr);        
    }

    return 0xff000000 | (r << 16) | (g << 8) | b;    
}

// ===========================================================================
// Makes in info banner to be put at the top of files for future reference
std::string makeBanner( int argc, char *argv[]  )
{
    std::string banner;

    // current date/time
    time_t rawtime;
    struct tm *timeinfo;
    char buf[80];

    time( &rawtime );
    timeinfo = localtime( &rawtime );

    strftime( buf, 80, "%c", timeinfo );    

    // TODO: version info for gfxpack

    banner += std::string("Generated by 'gfxpack' on ") + buf + " using this command line:\n";
    for (int i=0; i < argc; i++)
    {
        banner += std::string(argv[i]) + " ";
    }
    
    banner += "\n";
    
    return banner;    
}


// ===========================================================================
void outputFontInfo( std::vector<Chip*> &chips, std::string filename, std::string banner )
{
    FILE *fp = fopen( filename.c_str(), "wt" );
    
    std::stringstream ss(banner);
    std::string line;

    // add banner info
    while (std::getline( ss, line ))
    {
        fprintf( fp, "# %s\n", line.c_str() );        
    }

    // helpful comment for reference
    fprintf(fp, "\n# char pxlsize baseline x y w h\n" );

    for (std::vector<Chip*>::iterator ci = chips.begin();
         ci != chips.end(); ++ci )
    {
        Chip *c = (*ci);
        
        fprintf( fp, "%d %d %d %d %d %d %d\n",
                 c->m_char,
                 c->m_pxlsize,
                 c->m_baseline,
                 c->m_xpos, c->m_ypos,
                 c->m_width, c->m_height );
    }
    fclose( fp );    
    
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

    // colors
    unsigned long bgColor = 0x00000000;
    unsigned long borderColor = 0xff000000;
    unsigned long fgColor = 0xffffffff;    

    // list of font names
    std::vector<std::string> fontNames;    
    
    int ndx = 1;
    while (ndx < argc)
    {
        std::string arg = argv[ndx++];
        if ((arg=="--font")||(arg=="-f"))
        {
            // make sure there is an arg available
            if (ndx+1 > argc) errorMsg( "--font requires argument." );
            
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

            // get charmap
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
                                                   *ch, borderWidth,
                                                   fgColor, bgColor, borderColor );
                glyphChip->m_pxlsize = pxlSize;
                
                m_chipsToPack.push_back( glyphChip );
            }
        }
        else if ((arg=="-o")||(arg=="--out"))
        {
            // make sure there is an arg available
            if (ndx+1 > argc) errorMsg( "--out requires argument." );            
            
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
            if (ndx+1 > argc) errorMsg( "--charset requires argument." );
            
            std::string pattern = argv[ndx++];
            charSet = makeCharset( pattern );
        }
        else if ((arg=="--border") || (arg=="-b"))
        {
            if (ndx+1 > argc) errorMsg( "--border requires argument." );
            borderWidth = atoi( argv[ndx++] );
        }
        else if ((arg=="-fg") || (arg=="--foreground"))
        {
            if (ndx+1 > argc) errorMsg( "--foreground requires argument." );
            fgColor = parseColor( argv[ndx++] );
        }    
        else if ((arg=="-bg") || (arg=="--background"))
        {
            if (ndx+1 > argc) errorMsg( "--background requires argument." );
            bgColor = parseColor( argv[ndx++] );
        }    
        else if ((arg=="-bc") || (arg=="--bordercol"))
        {
            if (ndx+1 > argc) errorMsg( "--bordercol requires argument." );
            borderColor = parseColor( argv[ndx++] );
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
    FpImage *outImg = packChips( m_chipsToPack, bgColor ); // porkChops?
    
    printf("Saving font image %s...\n", outFontImg.c_str() );

    // save the resulting image
    outImg->writePng( outFontImg.c_str() );

    // make a info banner for the text outputs
    std::string banner = makeBanner( argc, argv );
    printf ("Banner:\n%s\n", banner.c_str() );

    // Output finfo if requested
    if (!outFinfoFile.empty())
    {
        printf("Saving .finfo file %s...\n", outFinfoFile.c_str() );
        outputFontInfo( m_chipsToPack, outFinfoFile, banner );        
    }
    

    
    
}
