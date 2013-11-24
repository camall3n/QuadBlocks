//
//  soundboard.cpp
//  QuadBlocks
//
//  Created by Cam Allen on 11/10/13.
//  Copyright (c) 2013 Cam Allen. All rights reserved.
//

#include "soundboard.h"

#include <boost/format.hpp>
#include <iostream>
#include <string>

extern int myrandom(int i);

const int nMusicTracks = 14;

MyMusic::MyMusic() :
    Music(),
    musicFile("worlds%02i.ogg")
{
    srand ( (unsigned int) time(NULL) );
    musicIndex = (rand()%nMusicTracks)+1;
    
    musicFile % musicIndex;// insert the index into the formatter
    openFromFile(musicFile.str().c_str());
}

bool MyMusic::onGetData(SoundStream::Chunk& data)
{
    bool running = sf::Music::onGetData(data);
    if(!running) {
        onMusicEnd();//attempt to continue with next song
        running = sf::Music::onGetData(data);
    }
    return running;
}

void MyMusic::onMusicEnd()
{
    musicIndex = ((musicIndex)%nMusicTracks)+1;
    musicFile % (musicIndex);
    openFromFile(musicFile.str().c_str());
}

Soundboard::Soundboard()
{
    LoadBuffers();
    InitializeSounds();
    InitializeMusic();
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
    loadBuffer(buffer.gameOver, "game-over.wav");
    
    loadBuffer(buffer.singleWhisper, "single.wav");
    loadBuffer(buffer.doubleWhisper, "double.wav");
    loadBuffer(buffer.tripleWhisper, "triple.wav");
    loadBuffer(buffer.quadWhisper, "tetris.wav");
    loadBuffer(buffer.allClearWhisper, "all-clear.wav");
    
    loadBuffer(buffer.move, "move.wav");
    loadBuffer(buffer.rotate, "rotate2.wav");
    loadBuffer(buffer.drop, "drop.wav");
    
    loadBuffer(buffer.pause, "pause.wav");
    loadBuffer(buffer.menu, "menu.wav");
    loadBuffer(buffer.menuSelect, "menu-select.wav");
}

void Soundboard::InitializeSounds()
{
    sound.lineClear.setBuffer(buffer.lineClear);
    sound.levelUp.setBuffer(buffer.levelUp);
    sound.gameOver.setBuffer(buffer.gameOver);
    
    sound.singleWhisper.setBuffer(buffer.singleWhisper);
    sound.doubleWhisper.setBuffer(buffer.doubleWhisper);
    sound.tripleWhisper.setBuffer(buffer.tripleWhisper);
    sound.quadWhisper.setBuffer(buffer.quadWhisper);
    sound.allClearWhisper.setBuffer(buffer.allClearWhisper);
    
    sound.move.setBuffer(buffer.move);
    sound.rotate.setBuffer(buffer.rotate);
    sound.drop.setBuffer(buffer.drop);
    
    sound.pause.setBuffer(buffer.pause);
    sound.menu.setBuffer(buffer.menu);
    sound.menuSelect.setBuffer(buffer.menuSelect);
}

void Soundboard::InitializeMusic()
{
    music.setVolume(25);
}


void Soundboard::LineClear() {
    sound.lineClear.play();
}
void Soundboard::LevelUp() {
    sound.levelUp.play();
}
void Soundboard::GameOver() {
    sound.gameOver.play();
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
void Soundboard::Quad() {
    sound.quadWhisper.play();
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
    if (sound.menuSelect.getStatus() != sf::Sound::Status::Playing) {
        sound.pause.play();
    }
}
void Soundboard::Menu() {
    sound.menu.play();
}
void Soundboard::MenuSelect() {
    sound.menuSelect.play();
}


void Soundboard::PlayMusic() {
    if (music.getStatus() != sf::Music::Status::Playing) {
        music.play();
    }
}
void Soundboard::PauseMusic() {
    if (music.getStatus() != music.Paused &&
        music.getStatus() != music.Stopped) {
        music.pause();
    }
}