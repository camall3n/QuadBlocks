//
//  scorekeeper.cpp
//  Tetris
//
//  Created by Cam Allen on 7/9/13.
//  Copyright (c) 2013 Cam Allen. All rights reserved.
//

#include "scorekeeper.h"

ScoreKeeper::ScoreKeeper() :
    score(0),
    lines(10),
    level(1),
    combo(0),
    difficult(0)
{
    
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


void ScoreKeeper::linesCleared(int lines)
{
    int points = 0;
    
    switch (lines) {
        case 4: //tetris
            points += 800*level;
            break;
        case 3: //triple
            points += 500*level;
            break;
        case 2: //double
            points += 300*level;
            break;
        case 1: //single
            points += 100*level;
            break;
        case 0: //none
            break;
        default:
            std::cerr << "Invalid arg to ScoreKeeper::linesCleared(int)" << std::endl;
    }
    
    // Process difficult moves
    if (lines == 4) {
        difficult++;// clear difficult, except on tetris
        if (difficult > 1)
            points = points*3/2;
    }
    else if (lines > 0) {
        difficult = 0;
    }
    
    // Process combos
    if (lines>0) {
        points += 50 * combo * level;
        combo++;
    } else {
        combo = 0;
    }
    
    updateScore(points);
    updateLines(lines);
}

void ScoreKeeper::tSpin(int lines, bool kick)
{
    int points = 0;
    switch (lines) {
        case 3: //triple
            points += 1600*level; // no-kick is impossible for t-spin triple
            break;
        case 2: //double
            points += (kick) ? 1200*level : 1200*level;
            break;
        case 1: //single
            points += (kick) ? 200*level : 800*level;
            break;
        case 0: //none
            points += (kick) ? 100*level : 400*level;
            break;
        default:
            std::cerr << "Invalid args to ScoreKeeper::tSpin(int, bool)" << std::endl;
    }
    
    // Process difficult moves
    if (lines > 0) {
        difficult++;
        if (difficult > 1)
            points = points*3/2;
    }
    
    // Process combos
    if (lines > 0) {
        points += 50 * combo * level;
        combo++;
    }
    else {
        combo = 0;
    }
    
    updateScore(points);
    updateLines(lines);
}

void ScoreKeeper::hardDrop(int distance) {
    updateScore(2*distance);
}

void ScoreKeeper::softDrop(int distance) {
    updateScore(1*distance);
}

void ScoreKeeper::resetCombo() {
    combo = 0;
}

void ScoreKeeper::updateScore(int points)
{
    score += points;
    if (points > 0) {
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
                this->lines += 10 + 5*level;
                updateLevel();
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
