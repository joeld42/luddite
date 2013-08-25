#include <stdio.h>

//#include <GLee.h>
//#include <glsw.h>
//
#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif


#include <SDL.h>
#include <SDL_endian.h>

// Game
#include "test_shell.h"
#include "test_scene.h"

#ifndef WIN32
#define _stricmp strcasecmp
#endif

int main( int argc, char *argv[] )
{
//    printf("Game Data Dir: %s\n", getResourceDir().c_str() );
    
	// Initialize SDL
	if (SDL_Init( SDL_INIT_NOPARACHUTE | SDL_INIT_VIDEO ) < 0 )
	{
		printf("Unable to init SDL: %s\n", SDL_GetError() );
		exit(1);
	}
    
	// crappy arg parse
    const char *sceneName = "mesh";
	Uint32 mode_flags = SDL_OPENGL;
	for (int i=1; i < argc; i++)
	{
        if (argv[i][0]!='-')
        {
            // No leading -, treat as a scene name
            sceneName = argv[i];
        }
		else if (!_stricmp( argv[i], "-fullscreen"))
		{
			mode_flags |= SDL_FULLSCREEN;
		}
	}


    // TODO: flag to control this
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute( SDL_GL_MULTISAMPLESAMPLES, 4 );
    
    
	if (SDL_SetVideoMode( 800, 600, 32, mode_flags ) == 0 )
	{
		printf( "Unable to set video mode: %s\n", SDL_GetError() ) ;
		exit(1);
	}
    
	SDL_WM_SetCaption( "Luddite Test App", NULL );
    
    TestApp::TestAppShell *game = new TestApp::TestAppShell();

    game->initWithScene( sceneName );
    
    // init graphics
    // FIXME: screen size
    glViewport( 0, 0, 800, 600 );
    
	//=====[ Main loop ]======
	bool done = false;
	Uint32 ticks = SDL_GetTicks(), ticks_elapsed, sim_ticks = 0;
	while(!done)
	{
		SDL_Event event;
        
		while (SDL_PollEvent( &event ) )
		{
			switch (event.type )
			{
				case SDL_KEYDOWN:
					switch( event.key.keysym.sym )
                {
                    case SDLK_ESCAPE:
                        done = true;
                        break;
                        
                    default:
//                        game->keypress( event.key.keysym.sym );
                        break;
                }
                    break;
                    
				case SDL_MOUSEMOTION:
//                    game->mouseMotion( event.motion.xrel, event.motion.yrel );
					break;
                    
				case SDL_MOUSEBUTTONDOWN:
//					game->mouseButton( event.button );
					break;
                    
				case SDL_MOUSEBUTTONUP:
//					game->mouseButton( event.button );
					break;
                    
				case SDL_QUIT:
					done = true;
					break;
                    
                default:
                    break;
			}
		}
        
        // Do continuous keys
		Uint8 *keyState = SDL_GetKeyState( NULL );
        
		// convert to btn mask
//		Uint32 btnMask = 0;
//		btnMask |= (keyState[SDLK_LEFT]||keyState[SDLK_a])?BTN_LEFT:0;
//		btnMask |= (keyState[SDLK_RIGHT]||keyState[SDLK_d])?BTN_RIGHT:0;
		
		// Timing
//		ticks_elapsed = SDL_GetTicks() - ticks;
//		ticks += ticks_elapsed;
//        
//		// fixed sim update
//		sim_ticks += ticks_elapsed;
//		while (sim_ticks > STEPTIME)
//		{
//			sim_ticks -= STEPTIME;
//            
//			//printf("update sim_ticks %d ticks_elapsed %d\n", sim_ticks, ticks_elapsed );
//			game->updateSim( (float)STEPTIME / 1000.0f );
//		}
//        
//		// redraw as fast as possible
//		float dtRaw = (float)(ticks_elapsed) / 1000.0f;
        
        if (game->update())
        {
            game->render();
        }
        else
        {
            done = true;
        }
        
		SDL_GL_SwapBuffers();
        
		// Call this once a frame if using tweakables
        //ReloadChangedTweakableValues();
	}
    
    // Shut down game
//    game->shutdown();
//    delete game;
    
    // Restore SDL stuff
    SDL_ShowCursor( true );
	SDL_WM_GrabInput( SDL_GRAB_OFF );
    
    // Shutdown glsw
//    glswShutdown();
    
    return 0;
}
