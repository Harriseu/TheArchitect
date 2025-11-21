/*******************************************************************************
 * Audio.h - SFML Audio Integration
 *
 * REQUIREMENT: Use an Audio Library (SFML) for Music and Sound Effects
 *
 * This module handles all audio playback in the game:
 * - Background music for different sectors
 * - Sound effects for combat, hacking, and events
 * - Volume control and audio settings
 *
 * Note: If SFML is not available, audio functions become no-ops
 * to allow the game to run without audio support.
 *
 * Author: Alif Harriz Jeffery | 102782711
 * Course: COS30008 Data Structures and Patterns
 ******************************************************************************/

#ifndef AUDIO_H
#define AUDIO_H

#include <string>
#include <iostream>

// Check if SFML is available
#ifdef SFML_AVAILABLE
#include <SFML/Audio.hpp>
#endif

/*******************************************************************************
 * Audio Manager - Handles all game audio
 *
 * Features:
 * - Background music with looping
 * - Sound effect playback
 * - Volume control
 * - Audio enable/disable
 ******************************************************************************/
class AudioManager {
private:
#ifdef SFML_AVAILABLE
    sf::Music backgroundMusic;
    sf::SoundBuffer* soundBuffers[10];  // Preallocated buffers
    sf::Sound* activeSounds[5];          // Active sound effects
#endif

    bool audioEnabled;
    bool musicEnabled;
    bool sfxEnabled;
    float masterVolume;
    float musicVolume;
    float sfxVolume;

    std::string currentMusicTrack;
    int activeSoundCount;

    // Audio file paths
    std::string audioPath;

    // Private constructor (Singleton)
    AudioManager();

public:
    // Singleton access
    static AudioManager& getInstance();

    // Destructor
    ~AudioManager();

    // Prevent copying
    AudioManager(const AudioManager&) = delete;
    AudioManager& operator=(const AudioManager&) = delete;

    // Initialization
    bool initialize(const std::string& audioDirectory);
    void shutdown();

    // Music Control
    bool playMusic(const std::string& filename, bool loop = true);
    void stopMusic();
    void pauseMusic();
    void resumeMusic();
    bool isMusicPlaying() const;

    // Sound Effects
    bool playSound(const std::string& filename);
    void stopAllSounds();

    // Predefined sound effects (for convenience)
    void playCombatStart();
    void playDamageSound();
    void playHealSound();
    void playLevelUpSound();
    void playHackSuccess();
    void playHackFail();
    void playMenuSelect();
    void playMenuBack();
    void playEventTrigger();
    void playVictorySound();
    void playDefeatSound();

    // Volume Control
    void setMasterVolume(float volume);     // 0.0 to 1.0
    void setMusicVolume(float volume);
    void setSFXVolume(float volume);
    float getMasterVolume() const;
    float getMusicVolume() const;
    float getSFXVolume() const;

    // Audio Toggle
    void enableAudio(bool enable);
    void enableMusic(bool enable);
    void enableSFX(bool enable);
    bool isAudioEnabled() const;
    bool isMusicEnabled() const;
    bool isSFXEnabled() const;

    // Debug
    void displayStatus() const;
};

/*******************************************************************************
 * Convenience functions for global access
 ******************************************************************************/
void playBackgroundMusic(const std::string& track);
void playSoundEffect(const std::string& sound);
void stopAllAudio();

#endif // AUDIO_H