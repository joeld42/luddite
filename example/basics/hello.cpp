#include <stdio.h>

// EASTL
#include <EASTL/string.h>

// Luddite 
#include <luddite/core/debug.h>
#include <luddite/core/handle.h>
#include <luddite/core/texture.h>
#include <luddite/core/gbuff.h>

// Opengl
#include <luddite/platform/gl.h>

// SDL
#include <SDL.h>
#include <SDL_endian.h>

using namespace luddite;

// 30 ticks per sim frame 
#define STEPTIME (33)

// Game globals
luddite::TextureDB g_texDB;
luddite::HTexture g_htexGrid;

struct TestVert
{
    float pos[3];
    float st[2];

    // required, describe our layout to opengl
    static void bind()
    {
        BIND_VERTEX( TestVert, pos );
        BIND_TEXTURE_COORD( TestVert, st );        
        
    }

    MAKE_BINDABLE( TestVert );    
};

GBuff<TestVert> gbuff;

// ===========================================================================
void hello_init()
{
    DBG::info( "Hello init\n" );
    
    g_htexGrid = luddite::TextureDB::singleton().getTexture( "gamedata/ash_uvgrid03-1.png" );    

    CHECKGL( "before init..." );    

    // populate gbuff
    TestVert *tv = gbuff.addTriangles( 2 );
    
    tv[0].st[0] = 0; tv[0].st[1] = 0;
    tv[0].pos[0] = 100.0; tv[0].pos[1] = 100.0; tv[0].pos[2] = 0.0;

    tv[1].st[0] = 1; tv[1].st[1] = 0;
    tv[1].pos[0] = 500.0; tv[1].pos[1] = 100.0; tv[1].pos[2] = 0.0;

    tv[2].st[0] = 1; tv[2].st[1] = 1;
    tv[2].pos[0] = 500.0; tv[2].pos[1] = 500.0; tv[2].pos[2] = 0.0;


    tv[3].st[0] = 1; tv[3].st[1] = 1;
    tv[3].pos[0] = 500.0; tv[3].pos[1] = 500.0; tv[3].pos[2] = 0.0;

    tv[4].st[0] = 0; tv[4].st[1] = 0;
    tv[4].pos[0] = 100.0; tv[4].pos[1] = 100.0; tv[4].pos[2] = 0.0;

    tv[5].st[0] = 0; tv[5].st[1] = 1;
    tv[5].pos[0] = 100.0; tv[5].pos[1] = 500.0; tv[5].pos[2] = 0.0;
    
    gbuff.update();
    CHECKGL( "after init..." );    
}

// ===========================================================================
void hello_redraw()
{    
    luddite::TextureDB &texDB = luddite::TextureDB::singleton();    
    
    GLuint texId = texDB.getTextureId( g_htexGrid );    

    glClearColor( 0.2f, 0.2f, 0.4f, 1.0f );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );    

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glOrtho( 0, 800, 0, 600, -1.0, 1.0 );    
    
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();    

    glDisable( GL_CULL_FACE );
    CHECKGL( "prep redraw" );
    
    glColor3f( 1.0, 1.0, 1.0 );
    glEnable( GL_TEXTURE_2D );
    CHECKGL( "enable texture" );

    // Draw the gbuff
    gbuff.draw();    

#if 0
    glBindTexture( GL_TEXTURE_2D, texId );

    glBindBuffer( GL_ARRAY_BUFFER, gbuff.vbo() );    

    glEnableClientState( GL_VERTEX_ARRAY );
    glVertexPointer( 3, GL_FLOAT, sizeof(TestVert), 0 );
    CHECKGL( "enable vert state" );    
    
    glEnableClientState( GL_TEXTURE_COORD_ARRAY );
    glTexCoordPointer( 2, GL_FLOAT, sizeof( TestVert), 
                       (void*)(3*sizeof(GLfloat)) );
    
    glDrawArrays( GL_TRIANGLES, 0, gbuff.numVerts() );
    CHECKGL( "draw arrays\n" );
#endif

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
                            
                        default:
                            // ignore
                            break;
                    }
                    break;

                case SDL_QUIT:
                    done = true;
                    break;      
                default:
                    // ignore
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





