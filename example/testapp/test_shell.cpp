
#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif


#include "test_shell.h"
#include "scene.h"
#include "scene_collect.h"
#include "scene_mesh.h"

using namespace luddite;
using namespace TestApp;

 TestApp::TestAppShell::TestAppShell()
 {
     // Register scenes
     m_scenes.push_back( new SceneCollect() );
     m_scenes.push_back( new SceneMesh() );

 }

 void TestApp::TestAppShell::commonInit()
 {
     m_gameLoop = new GameLoop();
 }

void TestApp::TestAppShell::initWithScene( const char *sceneName )
{
    commonInit();

    for (TestApp::Scene *scene : m_scenes )
    {
        if (scene->sceneName() == sceneName)
        {
            printf( "Using scene %s\n", scene->sceneName().c_str() );
            m_scene = scene;
            m_scene->init();
            return;
        }
    }

    printf( "Didn't find scene %s", sceneName);
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
//    AssertPtr( m_scene );

//    glClearColor( 0.2f, 0.2f, 1.0f, 1.0f );
//    glClear( GL_COLOR_BUFFER_BIT );

    m_scene->render();
}

void TestApp::TestAppShell::updateFixed( float dt )
{
//    AssertPtr( m_scene );

    //m_scene->updateFixed(dt);
}

void TestApp::TestAppShell::updateDynamic( float dt )
{
//    AssertPtr( m_scene );

    m_scene->updateDynamic(dt);
}







