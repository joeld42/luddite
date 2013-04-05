#ifndef LUDDITE_TEST_SHELL_H
#define LUDDITE_TEST_SHELL_H

#import <luddite/common/debug.h>
#import <luddite/game/gameloop.h>

namespace TestApp
{

// Common game-like stuff. Platform specific code can use this and feed it
// input and call the game loop and stuff.
    class TestAppShell
    {
    public:
        TestAppShell();

        void init();

        // mainloop stuff
        bool update();
        void render();

    private:
        void updateFixed( float dt );
        void updateDynamic( float dt );
        
        luddite::GameLoop *m_gameLoop;
    };


};

#endif
