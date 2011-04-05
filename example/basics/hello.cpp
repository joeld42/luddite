#include <stdio.h>

// EASTL
#include <EASTL/string.h>

// Luddite 
#include <luddite/core/debug.h>
#include <luddite/core/handle.h>
//#include <luddite/core/texture.h>

// SDL
#include <SDL.h>
#include <SDL_endian.h>

// opengl
#include <GL/gl.h>
#include <GL/glu.h>

using namespace luddite;

// 30 ticks per sim frame 
#define STEPTIME (33)

// Game globals
//luddite::TextureDB g_texDB;

// ===========================================================================
void hello_init()
{
    DBG::info( "Hello init\n" );
}

// ===========================================================================
void hello_redraw()
{
    glClearColor( 0.2f, 0.2f, 0.4f, 1.0f );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );    

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glOrtho( 0, 800, 0, 600, -1.0, 1.0 );
    
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    
    glColor3f( 1.0, 1.0, 1.0 );    
    glBegin( GL_QUADS );

    glVertex3f( 100, 100, 0.0 );
    glVertex3f( 500, 100, 0.0 );
    glVertex3f( 500, 500, 0.0 );
    glVertex3f( 100, 500, 0.0 );

    glEnd();
    
}


// ---------------------------------------------------------------------------
int main( int argc, char *argv[] )
{
    eastl::string s("hello");

    DBG::info( "Hello luddite\n" );    

    // Initialize SDL
    if (SDL_Init( SDL_INIT_NOPARACHUTE | SDL_INIT_VIDEO ) < 0 )
    {
        DBG::error( "Unable to init SDL: %s\n", SDL_GetError() );
        exit(1);        
    }

    if (SDL_SetVideoMode( 800, 600, 32, SDL_OPENGL )==0)
    {
        DBG::error( "Unable to set video mode: %s\n", SDL_GetError() );
        exit(1);        
    }        
    
    SDL_WM_SetCaption( "Hello Luddite", NULL );

    // init graphics
    glViewport( 0, 0, 800, 600 );
    
    bool done = false;
    Uint32 ticks = SDL_GetTicks(), ticks_elapsed, sim_ticks=0;

    hello_init();    
    
    while (!done)
    {
        SDL_Event event;
        
        while (SDL_PollEvent( &event ))
        {
            switch( event.type )
            {
                case SDL_KEYDOWN:
                    switch( event.key.keysym.sym )
                    {
                        case SDLK_ESCAPE:
                            done = true;
                            break;                        
                    }
                    break;

                case SDL_QUIT:
                    done = true;
                    break;                    
            }
            
        }


        // timing
        ticks_elapsed = SDL_GetTicks() - ticks;
        ticks += ticks_elapsed;
        // fixed sim update
		sim_ticks += ticks_elapsed;
		while (sim_ticks > STEPTIME) 
		{
			sim_ticks -= STEPTIME;						

			//demo_updateSim( (float)STEPTIME / 1000.0f );
		}	

		// redraw as fast as possible		
		float dtRaw = (float)(ticks_elapsed) / 1000.0f;
				
		//demo_updateFree( dtRaw ); 
        hello_redraw();        

		SDL_GL_SwapBuffers();
        
    }
    
    

    return 1;
}





