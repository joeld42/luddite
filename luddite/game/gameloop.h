#ifndef LUDDITE_GAMELOOP_H
#define LUDDITE_GAMELOOP_H

//
//  gameloop.h
//  luddite_ios
//
//  Created by Joel Davis on 7/4/12.
//  Copyright (c) 2012 Joel Davis. All rights reserved.
//

// Utility class to manage a game loop.
//
// Example usage in a main loop:
//
//
//    GameLoop mainloop;
//
//    while (1)
//    {
//        // Get inputs, etc..
//        ...
//
//        TimeInterval dt;
//        while ((dt=mainloop.simStep() >= 0.0)
//        {
//           // DO GAME SIM STEP HERE
//           
//           // dt will always be a fixed value here (1.0/stepTime) for stability
//           // but it's a good idea to use dt anyways.
//           
//           mypos = mypos + myvel*dt;
//        }
//            
//        // DO DYNAMIC SYM STEP HERE
//               
//        // Good for stuff like particles that works well with a variable timestep.
//        // Avoid putting gameplay-dependant stuff here
//        dt = mainloop.dynamicStep();
//        shinypos = shinypos + shinyvel*dt;
//               
//        // Now render the next frame
//        ...
//    }
//

namespace luddite
{

typedef double TimeInterval;
    
// approx 30 fps sim steps
#define kDefaultStepTime (33.0)
    
class GameLoop
{
public:
    
    GameLoop( TimeInterval stepTime=kDefaultStepTime );
    
    // Returns a fixed dt timestep if there are outstanding
    // sim steps needed. Returns -1.0 when no more sim steps remain 
    // for this frame
    TimeInterval simStep();
    
    // Returns a varying timestep with the elapsed time since the
    // last step frame. Note that time interval is measured from the first
    // call of simStep() for this frame, not from this call itself (so 
    // the processing time of any sim steps will have no effect on the timing
    // of this frame)
    TimeInterval dynamicStep();
    
protected:
    TimeInterval m_stepTime;
    
    TimeInterval m_simTime;
    TimeInterval m_timeLeftover;
    TimeInterval m_dtFixed;
    TimeInterval m_dtRaw;
};
    
}; // namespace luddite

#endif
