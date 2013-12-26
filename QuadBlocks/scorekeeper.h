//
//  scorekeeper.h
//  QuadBlocks
//
//  Created by Cam Allen on 7/9/13.
//  Copyright (c) 2013 Cam Allen. All rights reserved.
//

#ifndef __QuadBlocks__scorekeeper__
#define __QuadBlocks__scorekeeper__

#include <boost/signals2.hpp>
namespace bs2 = boost::signals2;

const int MAX_LEVEL = 15;

class ScoreKeeper
{
public:
    ScoreKeeper();
    
    void reset();
    
    struct signalsList {
        bs2::signal<void (int)> scoreChanged;
        bs2::signal<void (int)> newPoints;
        bs2::signal<void (int)> linesChanged;
        bs2::signal<void (int)> levelChanged;
    } signal;
    
    int getScore();
    int getLines();
    int getLevel();
    
    void queueBravo();
    void linesCleared(int lines);
    void tSpin(int lines, bool kick);
    void hardDrop(int distance);
    void softDrop(int distance);
    
    int scoreCascade(int lines);// returns current cascade chain length
    void resetCascade();
    
private:
    int score;
    int lines;
    int level;
    int combo;
    int difficult;
    int bravo;
    
    int cascade;
    
    void updateScore(int points);
    void updateLines(int lines);
    void updateLevel();
};

#endif /* defined(__QuadBlocks__scorekeeper__) */
