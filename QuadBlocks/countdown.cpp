//
//  countdown.cpp
//  QuadBlocks
//
//  Created by Cam Allen on 12/12/13.
//  Copyright (c) 2013 Cam Allen. All rights reserved.
//

#include "countdown.h"
#include <algorithm>
#include <boost/lexical_cast.hpp>
#include <cmath>

Countdown::Countdown()
 : totalSeconds(0),
   cachedSeconds(0)
{
}

void Countdown::setTime(int seconds) {
    totalSeconds = std::max(seconds, 0);
    cachedSeconds = totalSeconds;
    reset();
}

void Countdown::setTime(int minutes, int seconds) {
    setTime(60 * minutes + seconds);
}

int Countdown::getSecondsRemaining() {
    int secondsElapsed = floor(Timer::getTime());
    int secondsRemaining = std::max(totalSeconds - secondsElapsed, 0);
    cachedSeconds = secondsRemaining;
    
    return secondsRemaining;
}

std::string Countdown::getTimeString() {
    int secondsRemaining = getSecondsRemaining();
    int minutes = secondsRemaining / 60;
    int seconds = secondsRemaining % 60;
    
    std::string str;
    str = boost::lexical_cast<std::string>(minutes) +
          ":" + ((seconds < 10) ? "0" : "" ) +
          boost::lexical_cast<std::string>(seconds);
    
    return str;
}

void Countdown::start() {
    Timer::start();
    cachedSeconds = totalSeconds;
}

void Countdown::stop() {
    Timer::stop();
    cachedSeconds = totalSeconds;
}

void Countdown::reset() {
    stop();
}


bool Countdown::didChange() {
    // grab cachedSeconds since it will change when calling getSecondsRemaining
    int prevSeconds = cachedSeconds;

    int currSeconds = getSecondsRemaining();
    if (currSeconds == prevSeconds) {
        return false;
    }
    return true;
}

bool Countdown::isDone() {
    if (this->getSecondsRemaining() > 0) {
        return false;
    }
    return true;
}

