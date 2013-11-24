//
//  soundboard.h
//  QuadBlocks
//
//  Created by Cam Allen on 11/10/13.
//  Copyright (c) 2013 Cam Allen. All rights reserved.
//

#ifndef __QuadBlocks__soundboard__
#define __QuadBlocks__soundboard__

#include <boost/format.hpp>
#include <SFML/Audio.hpp>

class MyMusic : public sf::Music
{
    bool onGetData(sf::SoundStream::Chunk& data);

public:
    MyMusic();
    void onMusicEnd();

private:
    int musicIndex;
    boost::format musicFile;
};

class Soundboard
{
public:
    Soundboard();
    
    void LineClear();
    void LevelUp();
    void GameOver();
    
    void Single();
    void Double();
    void Triple();
    void Quad();
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
        sf::SoundBuffer gameOver;
        
        sf::SoundBuffer singleWhisper;
        sf::SoundBuffer doubleWhisper;
        sf::SoundBuffer tripleWhisper;
        sf::SoundBuffer quadWhisper;
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
        sf::Sound gameOver;
        
        sf::Sound singleWhisper;
        sf::Sound doubleWhisper;
        sf::Sound tripleWhisper;
        sf::Sound quadWhisper;
        sf::Sound allClearWhisper;
        
        sf::Sound move;
        sf::Sound rotate;
        sf::Sound drop;
        
        sf::Sound pause;
        sf::Sound menu;
        sf::Sound menuSelect;
    } sound;
    
    void InitializeMusic();
    
    MyMusic music;
};

#endif /* defined(__QuadBlocks__soundboard__) */
