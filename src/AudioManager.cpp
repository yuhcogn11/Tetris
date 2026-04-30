/*
 * AudioManager.cpp
 * ----------------
 * Triển khai AudioManager cho SDL3_mixer 3.2.0.
 */

#include "AudioManager.h"
#include <iostream>

AudioManager::AudioManager()
    : initialized(false), bgmEnabled(true), sfxEnabled(true),
      mixer(nullptr), bgmTrack(nullptr), sfxTrack(nullptr),
      bgm(nullptr), moveSfx(nullptr) {}

AudioManager::~AudioManager() {
    shutdown();
}

bool AudioManager::init() {
    if (!MIX_Init()) {
        std::cerr << "MIX_Init failed: " << SDL_GetError() << std::endl;
        return false;
    }

    // Mở mixer device mặc định.
    mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr);
    if (!mixer) {
        std::cerr << "MIX_CreateMixerDevice failed: " << SDL_GetError() << std::endl;
        return false;
    }

    // Tạo các track để phát âm thanh.
    bgmTrack = MIX_CreateTrack(mixer);
    sfxTrack = MIX_CreateTrack(mixer);

    // Load file âm thanh.
    bgm = MIX_LoadAudio(mixer, "assets/sounds/nhacnen.wav", false);
    if (!bgm) {
        std::cerr << "Failed to load nhacnen.wav: " << SDL_GetError() << std::endl;
    }

    moveSfx = MIX_LoadAudio(mixer, "assets/sounds/move.wav", false);
    if (!moveSfx) {
        std::cerr << "Failed to load move.wav: " << SDL_GetError() << std::endl;
    }

    // Đặt âm lượng mặc định.
    setBGMVolume(0.5f);
    setSFXVolume(0.8f);

    initialized = true;
    return true;
}

void AudioManager::shutdown() {
    if (!initialized) return;

    if (bgmTrack) {
        MIX_StopTrack(bgmTrack, 0);
        MIX_DestroyTrack(bgmTrack);
        bgmTrack = nullptr;
    }

    if (sfxTrack) {
        MIX_StopTrack(sfxTrack, 0);
        MIX_DestroyTrack(sfxTrack);
        sfxTrack = nullptr;
    }

    if (bgm) {
        MIX_DestroyAudio(bgm);
        bgm = nullptr;
    }

    if (moveSfx) {
        MIX_DestroyAudio(moveSfx);
        moveSfx = nullptr;
    }

    if (mixer) {
        MIX_DestroyMixer(mixer);
        mixer = nullptr;
    }

    MIX_Quit();
    initialized = false;
}

void AudioManager::playBGM() {
    if (!initialized || !bgmEnabled || !bgm || !bgmTrack) return;

    MIX_SetTrackAudio(bgmTrack, bgm);
    
    SDL_PropertiesID props = SDL_CreateProperties();
    SDL_SetNumberProperty(props, MIX_PROP_PLAY_LOOPS_NUMBER, -1); // Lặp vô hạn
    MIX_PlayTrack(bgmTrack, props);
    SDL_DestroyProperties(props);
}

void AudioManager::stopBGM() {
    if (!initialized || !bgmTrack) return;
    MIX_StopTrack(bgmTrack, 0);
}

void AudioManager::pauseBGM() {
    if (!initialized || !bgmTrack) return;
    MIX_PauseTrack(bgmTrack);
}

void AudioManager::resumeBGM() {
    if (!initialized || !bgmEnabled || !bgmTrack) return;
    MIX_ResumeTrack(bgmTrack);
}

void AudioManager::setBGMVolume(float volume) {
    if (!initialized || !bgmTrack) return;
    MIX_SetTrackGain(bgmTrack, volume);
}

void AudioManager::playSFX(SoundType sound) {
    if (!initialized || !sfxEnabled || !sfxTrack) return;

    MIX_Audio* sfxToPlay = nullptr;
    switch (sound) {
    case SoundType::MOVE:
    case SoundType::ROTATE:
    case SoundType::LOCK:
    case SoundType::CLEAR_SINGLE:
    case SoundType::CLEAR_DOUBLE:
    case SoundType::CLEAR_TRIPLE:
    case SoundType::CLEAR_TETRIS:
    case SoundType::GAME_OVER:
        sfxToPlay = moveSfx;
        break;
    }

    if (sfxToPlay) {
        MIX_SetTrackAudio(sfxTrack, sfxToPlay);
        MIX_PlayTrack(sfxTrack, 0); // Phát 1 lần mặc định
    }
}

void AudioManager::setSFXVolume(float volume) {
    if (!initialized || !sfxTrack) return;
    MIX_SetTrackGain(sfxTrack, volume);
}

void AudioManager::toggleBGM() {
    setBGMEnabled(!bgmEnabled);
}

void AudioManager::toggleSFX() {
    setSFXEnabled(!sfxEnabled);
}

bool AudioManager::isBGMEnabled() const {
    return bgmEnabled;
}

bool AudioManager::isSFXEnabled() const {
    return sfxEnabled;
}

void AudioManager::setBGMEnabled(bool enabled) {
    if (bgmEnabled == enabled) return;
    bgmEnabled = enabled;

    if (bgmEnabled) {
        resumeBGM();
    } else {
        pauseBGM();
    }
}

void AudioManager::setSFXEnabled(bool enabled) {
    sfxEnabled = enabled;
}

void AudioManager::handleEvent(const SDL_Event& e) {
    if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN &&
        e.button.button == SDL_BUTTON_LEFT) {
        playSFX(SoundType::MOVE);
    }
}

void AudioManager::printSettings() {
    std::cout << "Audio Settings - BGM: " << (bgmEnabled ? "ON" : "OFF")
              << " | SFX: " << (sfxEnabled ? "ON" : "OFF") << std::endl;
}