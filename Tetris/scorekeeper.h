//
//  scorekeeper.h
//  Tetris
//
//  Created by Cam Allen on 7/9/13.
//  Copyright (c) 2013 Cam Allen. All rights reserved.
//

#ifndef __Tetris__scorekeeper__
#define __Tetris__scorekeeper__

#include <boost/signals2.hpp>
namespace bs2 = boost::signals2;

const int MAX_LEVEL = 15;

class ScoreKeeper
{
public:
    ScoreKeeper();
    
    struct signalsList {
        bs2::signal<void (int)> scoreChanged;
        bs2::signal<void (int)> linesChanged;
        bs2::signal<void (int)> levelChanged;
    } signal;
    
    int getScore();
    int getLines();
    int getLevel();
    
    void linesCleared(int lines);
    void tSpin(int lines, bool kick);
    void hardDrop(int distance);
    void softDrop(int distance);
    void resetCombo();
    
private:
    int score;
    int lines;
    int level;
    int combo;
    int difficult;
    
    void updateScore(int points);
    void updateLines(int lines);
    void updateLevel();
};

#endif /* defined(__Tetris__scorekeeper__) */
