#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>
#include "GameState.h"

class AudioManager {
private:
    bool initialized; 
    bool bgmEnabled;  
    bool sfxEnabled;  

    MIX_Mixer* mixer;     
    MIX_Track* bgmTrack;  
    MIX_Track* sfxTrack;  

    MIX_Audio* bgm;      
    MIX_Audio* moveSfx;  

public:
    AudioManager();
    ~AudioManager();

    bool init();
    void shutdown();

    void playBGM();                  
    void stopBGM();                  
    void pauseBGM();                 
    void resumeBGM();                
    void setBGMVolume(float volume); 

    void playSFX(SoundType sound);
    void setSFXVolume(float volume); 

    void toggleBGM();                        
    void toggleSFX();                        
    bool isBGMEnabled() const;
    bool isSFXEnabled() const;
    void setBGMEnabled(bool enabled);        
    void setSFXEnabled(bool enabled);

    void handleEvent(const SDL_Event& e);
    void printSettings(); 
};

#endif