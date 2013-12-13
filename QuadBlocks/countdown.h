//
//  countdown.h
//  QuadBlocks
//
//  Created by Cam Allen on 12/12/13.
//  Copyright (c) 2013 Cam Allen. All rights reserved.
//

#ifndef __QuadBlocks__countdown__
#define __QuadBlocks__countdown__

#include "timer.h"
#include <boost/signals2.hpp>
namespace bs2 = boost::signals2;
#include <string>

class Countdown : public Timer
{
public:
    Countdown();
    void setTime(int minutes, int seconds);
    void setTime(int seconds);
    int getSecondsRemaining();
    std::string getTimeString();

    void start();
    void stop();
    void reset();
    
    bool didChange();
    bool isDone();
    
private:
    int totalSeconds;
    int cachedSeconds;
};

#endif