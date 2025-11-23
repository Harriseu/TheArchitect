#ifndef AUDIO_H
#define AUDIO_H

#include <string>
#include <iostream>
#include <map>
#include <list>

// Check if SFML is available
#ifdef SFML_AVAILABLE
#include <SFML/Audio.hpp>
#endif

class AudioManager {
private:
#ifdef SFML_AVAILABLE
    sf::Music backgroundMusic;
    // Cache for sound buffers to avoid reloading files
    std::map<std::string, sf::SoundBuffer> soundBufferCache;
    // List to manage currently playing sound instances
    std::list<sf::Sound> activeSounds;
#endif

    bool audioEnabled;
    bool musicEnabled;
    bool sfxEnabled;
    float masterVolume;
    float musicVolume;
    float sfxVolume;

    std::string currentMusicTrack;
    std::string audioPath;

    // Maximum number of concurrent sound effects
    const unsigned int MAX_CONCURRENT_SOUNDS = 10;

    AudioManager();

    // Private helper to clean up finished sounds
    void cleanupFinishedSounds();

public:
    static AudioManager& getInstance();
    ~AudioManager();

    // Prevent copying
    AudioManager(const AudioManager&) = delete;
    AudioManager& operator=(const AudioManager&) = delete;

    bool initialize(const std::string& audioDirectory);
    void shutdown();

    bool playMusic(const std::string& filename, bool loop = true);
    void stopMusic();
    void pauseMusic();
    void resumeMusic();
    bool isMusicPlaying() const;

    bool playSound(const std::string& filename);
    void stopAllSounds();

    // Predefined sound effects
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

    void setMasterVolume(float volume);
    void setMusicVolume(float volume);
    void setSFXVolume(float volume);
    float getMasterVolume() const;
    float getMusicVolume() const;
    float getSFXVolume() const;

    void enableAudio(bool enable);
    void enableMusic(bool enable);
    void enableSFX(bool enable);
    bool isAudioEnabled() const;
    bool isMusicEnabled() const;
    bool isSFXEnabled() const;

    void displayStatus() const;
};

// Global convenience functions
void playBackgroundMusic(const std::string& track);
void playSoundEffect(const std::string& sound);
void stopAllAudio();

#endif // AUDIO_H