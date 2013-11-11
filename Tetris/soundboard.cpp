//
//  soundboard.cpp
//  Tetris
//
//  Created by Cam Allen on 11/10/13.
//  Copyright (c) 2013 Cam Allen. All rights reserved.
//

#include "soundboard.h"

#include <iostream>
#include <string>

Soundboard::Soundboard()
{
    LoadBuffers();
    InitializeSounds();
}

static void loadBuffer(sf::SoundBuffer &buf, std::string str) {
    if (!buf.loadFromFile(str.c_str())) {
        std::cerr << "Error: could not load buffer '" << str << "'";
    }
}

void Soundboard::LoadBuffers()
{
    loadBuffer(buffer.lineClear, "line-clear.wav");
    loadBuffer(buffer.levelUp, "level-up.wav");
    
    loadBuffer(buffer.singleWhisper, "single.wav");
    loadBuffer(buffer.doubleWhisper, "double.wav");
    loadBuffer(buffer.tripleWhisper, "triple.wav");
    loadBuffer(buffer.tetrisWhisper, "tetris.wav");
    loadBuffer(buffer.allClearWhisper, "all-clear.wav");
    
    loadBuffer(buffer.move, "move.wav");
    loadBuffer(buffer.rotate, "rotate.wav");
    loadBuffer(buffer.drop, "drop.wav");
    
    loadBuffer(buffer.pause, "pause.wav");
    loadBuffer(buffer.menu, "menu.wav");
    loadBuffer(buffer.menuSelect, "menu-select.wav");
}

void Soundboard::InitializeSounds()
{
    sound.lineClear.setBuffer(buffer.lineClear);
    sound.levelUp.setBuffer(buffer.levelUp);
    
    sound.singleWhisper.setBuffer(buffer.singleWhisper);
    sound.doubleWhisper.setBuffer(buffer.doubleWhisper);
    sound.tripleWhisper.setBuffer(buffer.tripleWhisper);
    sound.tetrisWhisper.setBuffer(buffer.tetrisWhisper);
    sound.allClearWhisper.setBuffer(buffer.allClearWhisper);
    
    sound.move.setBuffer(buffer.move);
    sound.rotate.setBuffer(buffer.rotate);
    sound.drop.setBuffer(buffer.drop);
    
    sound.pause.setBuffer(buffer.pause);
    sound.menu.setBuffer(buffer.menu);
    sound.menuSelect.setBuffer(buffer.menuSelect);
}


void Soundboard::LineClear() {
    sound.lineClear.play();
}
void Soundboard::LevelUp() {
    sound.levelUp.play();
}


void Soundboard::Single() {
    sound.singleWhisper.play();
}
void Soundboard::Double() {
    sound.doubleWhisper.play();
}
void Soundboard::Triple() {
    sound.tripleWhisper.play();
}
void Soundboard::Tetris() {
    sound.tetrisWhisper.play();
}
void Soundboard::AllClear() {
    sound.allClearWhisper.play();
}


void Soundboard::Move() {
    sound.move.play();
}
void Soundboard::Rotate() {
    sound.rotate.play();
}
void Soundboard::Drop() {
    sound.drop.play();
}


void Soundboard::Pause() {
    sound.pause.play();
}
void Soundboard::Menu() {
    sound.menu.play();
}
void Soundboard::MenuSelect() {
    sound.menuSelect.play();
}

