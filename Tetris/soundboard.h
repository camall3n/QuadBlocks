//
//  soundboard.h
//  Tetris
//
//  Created by Cam Allen on 11/10/13.
//  Copyright (c) 2013 Cam Allen. All rights reserved.
//

#ifndef __Tetris__soundboard__
#define __Tetris__soundboard__

#include <SFML/Audio.hpp>

class Soundboard
{
public:
    Soundboard();
    
    void LineClear();
    void LevelUp();
    
    void Single();
    void Double();
    void Triple();
    void Tetris();
    void AllClear();
    
    void Move();
    void Rotate();
    void Drop();
    
    void Pause();
    void Menu();
    void MenuSelect();
    
    void PlayMusic();
    void PauseMusic();
    
private:
    
    void LoadBuffers();
    
    struct bufferList {
        sf::SoundBuffer lineClear;
        sf::SoundBuffer levelUp;
        
        sf::SoundBuffer singleWhisper;
        sf::SoundBuffer doubleWhisper;
        sf::SoundBuffer tripleWhisper;
        sf::SoundBuffer tetrisWhisper;
        sf::SoundBuffer allClearWhisper;
        
        sf::SoundBuffer move;
        sf::SoundBuffer rotate;
        sf::SoundBuffer drop;
        
        sf::SoundBuffer pause;
        sf::SoundBuffer menu;
        sf::SoundBuffer menuSelect;
    } buffer;
    
    
    void InitializeSounds();
    
    struct soundList {
        sf::Sound lineClear;
        sf::Sound levelUp;
        
        sf::Sound singleWhisper;
        sf::Sound doubleWhisper;
        sf::Sound tripleWhisper;
        sf::Sound tetrisWhisper;
        sf::Sound allClearWhisper;
        
        sf::Sound move;
        sf::Sound rotate;
        sf::Sound drop;
        
        sf::Sound pause;
        sf::Sound menu;
        sf::Sound menuSelect;
    } sound;
    
};

#endif /* defined(__Tetris__soundboard__) */
