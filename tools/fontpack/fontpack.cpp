#include <string>

#include <ft2build.h>
#include FT_FREETYPE_H

int main( int argc, char *argv[] )
{
    FT_Library ftLibrary;
    FT_Init_FreeType( &ftLibrary );

    int ndx = 1;
    while (ndx < argc)
    {
        std::string arg = argv[ndx++];
        if (arg=="-font")
        {
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
        
    }    

}
