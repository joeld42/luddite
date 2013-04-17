//
// Created by joeld on 4/7/13.
//
// To change the template use AppCode | Preferences | File Templates.
//

#include <EASTL/string.h>

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#include "scene_collect.h"


eastl::string TestApp::SceneCollect::sceneName()
{
    return "collect";
}


void TestApp::SceneCollect::init()
{
}

void TestApp::SceneCollect::render()
{
    printf("in SceneCollect::render...\n");
    glClearColor( 0.2f, 1.0f, 0.0f, 1.0f );
    glClear( GL_COLOR_BUFFER_BIT );

}
