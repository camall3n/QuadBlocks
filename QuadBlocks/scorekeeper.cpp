//
//  scorekeeper.cpp
//  QuadBlocks
//
//  Created by Cam Allen on 7/9/13.
//  Copyright (c) 2013 Cam Allen. All rights reserved.
//

#include "scorekeeper.h"

const int START_LEVEL = 1;

ScoreKeeper::ScoreKeeper() :
    score(0),
    lines(5),
    level(START_LEVEL),
    combo(0),
    difficult(0),
    bravo(0)
{
}

void ScoreKeeper::reset()
{
    level = START_LEVEL;
    
    if (level > 1) {
        signal.levelChanged(level);
    }
}

int ScoreKeeper::getScore() {
    return score;
}

int ScoreKeeper::getLines() {
    return lines;
}

int ScoreKeeper::getLevel() {
    return level;
}

void ScoreKeeper::queueBravo() {
    bravo = 4;
}

void ScoreKeeper::linesCleared(int clears)
{
    int points = 0;
    int lines = 0;
    if (bravo>1) {
        lines = 0;
    }
    
    switch (clears) {
        case 4: //quad
            points = 800*level;
            lines  = 8;
            break;
        case 3: //triple
            points = 500*level;
            lines  = 5;
            break;
        case 2: //double
            points = 300*level;
            lines  = 3;
            break;
        case 1: //single
            points = 100*level;
            lines  = 1;
            break;
        case 0: //none
            break;
        default:
            std::cerr << "Invalid arg to ScoreKeeper::linesCleared(int)" << std::endl;
    }
    
    // Process difficult moves
    if (clears == 4) {
        difficult++;// clear difficult, except on quad
        if (difficult > 1) {
            points = points*3/2;
            lines = lines*3/2;
        }
    }
    else if (clears > 0) {
        difficult = 0;
    }
    
    // Process combos
    if (clears>0) {
        points += 50 * combo * level;
        combo++;
    } else {
        combo = 0;
    }
    
    points *= bravo;
    bravo = 1;
    
    updateScore(points);
    updateLines(lines);
}

void ScoreKeeper::tSpin(int clears, bool kick)
{
    int points = 0;
    int lines = 0;
    
    switch (clears) {
        case 3: //triple
            points = 1600*level; // no-kick is impossible for t-spin triple
            lines  = 16;
            break;
        case 2: //double
            points = 1200*level; // kick is apparently ignored for double
            lines  = 12;
            break;
        case 1: //single
            points = (kick) ? 200*level : 800*level;
            lines  = (kick) ? 2 : 8;
            break;
        case 0: //none
            points = (kick) ? 100*level : 400*level;
            lines  = (kick) ? 1 : 4;
            break;
        default:
            std::cerr << "Invalid args to ScoreKeeper::tSpin(int, bool)" << std::endl;
    }
    
    // Process difficult moves
    if (clears > 0) {
        difficult++;
        if (difficult > 1) {
            points = points*3/2;
            lines = lines*3/2;
        }
    }
    
    // Process combos
    if (clears > 0) {
        points += 50 * combo * level;
        combo++;
    }
    else {
        combo = 0;
    }
    
    points *= bravo;
    bravo = 1;
    
    updateScore(points);
    updateLines(lines);
}

void ScoreKeeper::hardDrop(int distance) {
    updateScore(2*distance);
}

void ScoreKeeper::softDrop(int distance) {
    updateScore(1*distance);
}

void ScoreKeeper::updateScore(int points)
{
    score += points;
    if (points > 0) {
        if (points > 1) {
            signal.newPoints(points);            
        }
        signal.scoreChanged(score);
    }
}

void ScoreKeeper::updateLines(int lines)
{
    if (this->lines == 0 && level == MAX_LEVEL) {
        
    }
    else if (lines > 0) {
        this->lines -= lines;
        if (this->lines <= 0) {
            if (level < MAX_LEVEL) {
                updateLevel();
                this->lines = 5*level;
            }
            else {
                this->lines = 0;
            }
        }
        signal.linesChanged(this->lines);
    }
}

void ScoreKeeper::updateLevel()
{
    level++;
    signal.levelChanged(level);
}
