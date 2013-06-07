//
//  timer.h
//  Tetris
//
//  Created by Cam Allen on 11/4/12.
//

#ifndef __Tetris__timer__
#define __Tetris__timer__

class Timer
{
public:
    Timer();
    
    //Timer actions
    void start();
    void stop();
    void pause();
    void unpause();
    
    double getTime();// Since start
    double getLapTime();// Since last getLapTime (or since start if no lap time)
    
    //Check the status of the timer
    bool isStarted();
    bool isPaused();
    
private:
    //The clock time when the timer started
    double startTime;
    
    //The time stored when the timer was paused
    double pausedTime;
    
    //The time stored when the last lapTime was requested
    double lapTime;
    
    //The timer status
    bool paused;
    bool started;
    
};

#endif
