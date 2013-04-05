
#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif


#include "test_shell.h"

using namespace luddite;

 TestApp::TestAppShell::TestAppShell()
 {
    init();
 }

 void TestApp::TestAppShell::init()
 {
     m_gameLoop = new GameLoop();
 }

 bool TestApp::TestAppShell::update()
 {
     // game loop
     double dt;
     while ((dt=m_gameLoop->simStep()) >= 0.0)
     {
         // DO GAME SIM STEP HERE
         
         // dt will always be a fixed value here (1.0/stepTime) for stability
         // but it's a good idea to use dt anyways.
         updateFixed( dt );
     }
     
     // DO DYNAMIC SYM STEP HERE
     
     // Good for stuff like particles that works well with a variable timestep.
     // Avoid putting gameplay-dependant stuff here
     dt = m_gameLoop->dynamicStep();     
     updateDynamic(dt);
     
     return true;
 }

void TestApp::TestAppShell::render()
{
    glClearColor( 0.2f, 0.2f, 1.0f, 1.0f );
    glClear( GL_COLOR_BUFFER_BIT );
}

void TestApp::TestAppShell::updateFixed( float dt )
{
    DBG::info( "Fixed timestep: %lf\n", dt );
}

void TestApp::TestAppShell::updateDynamic( float dt )
{
    DBG::info( "Dynamic timestep: %lf\n", dt );
    
}







