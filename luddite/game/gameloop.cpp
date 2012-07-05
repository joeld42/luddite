//
//  gameloop.cpp
//  luddite_ios
//
//  Created by Joel Davis on 7/4/12.
//  Copyright (c) 2012 Joel Davis. All rights reserved.
//

#include "gameloop.h"
#include "debug.h"

// TMP until platform time
#include <CoreFoundation/CoreFoundation.h>

using namespace luddite;

GameLoop::GameLoop( TimeInterval stepTime ) :
            m_stepTime(stepTime)
{
//    Assert( stepTime==0.0, "Steptime must be non-zero" );
    m_dtFixed = (1.0 / stepTime);
    m_simTime = 0;
}


TimeInterval GameLoop::simStep()
{
    // Is there time leftover on this frame?
    if (m_timeLeftover >= m_dtFixed)
    {
        // Yes, consume one frame worth of time and continue
        m_timeLeftover -= m_dtFixed;
        return m_dtFixed;
    }
    else
    {
        // Timing
        m_dtRaw = CFAbsoluteTimeGetCurrent() - m_simTime;
        m_simTime += m_dtRaw;
        
        // CBB: If the timeElapsed is too big (i.e. viewing an ad or
        // switching to another app, just skip this update
        
        if (m_dtRaw > 0.3) 
        {
            DBG::warn("TimeElapsed above threshold, skipping\n" ); 
            m_timeLeftover = 0.0; // use up all leftover time and return a single step
            return m_dtFixed;
        }
        
        // Update leftover time
        m_timeLeftover += m_dtRaw;
        
        return -1.0; // Done with this frame
    }
}

TimeInterval GameLoop::dynamicStep()
{
    // This gets filled in by simStep
    return m_dtRaw;
}


