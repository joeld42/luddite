#ifndef LUDDITE_TEST_SHELL_H
#define LUDDITE_TEST_SHELL_H

#import <luddite/common/debug.h>
#import <luddite/game/gameloop.h>

#import <EASTL/vector.h>

#import "scene.h"

namespace TestApp
{

// Common game-like stuff. Platform specific code can use this and feed it
// input and call the game loop and stuff.
class TestAppShell
{
public:
    TestAppShell();

    void commonInit();
    void initWithScene( const char *scene );

    // mainloop stuff
    bool update();
    void render();

private:
    void updateFixed( float dt );
    void updateDynamic( float dt );

    luddite::GameLoop *m_gameLoop;

    // registered test scenes
    eastl::vector<TestApp::Scene*> m_scenes;

    // current scene
    TestApp::Scene *m_scene;
};


};

#endif
