/*******************************************************************************
 * Audio.cpp - SFML Audio Integration Implementation
 *
 * REQUIREMENT: Use an Audio Library (SFML) for Music and Sound Effects
 *
 * Note: SFML audio is optional. If not available, functions become no-ops.
 *
 * Author: Digital Exodus Project
 * Course: COS30008 Data Structures and Patterns
 ******************************************************************************/

#include "../include/Audio.h"

/*******************************************************************************
 * AUDIO MANAGER IMPLEMENTATION
 ******************************************************************************/

// Private constructor
AudioManager::AudioManager()
    : audioEnabled(true), musicEnabled(true), sfxEnabled(true),
      masterVolume(1.0f), musicVolume(0.7f), sfxVolume(0.8f),
      currentMusicTrack(""), activeSoundCount(0), audioPath("") {

#ifdef SFML_AVAILABLE
    // Initialize sound buffers and sounds
    for (int i = 0; i < 10; i++) {
        soundBuffers[i] = nullptr;
    }
    for (int i = 0; i < 5; i++) {
        activeSounds[i] = nullptr;
    }
#endif
}

// Singleton instance
AudioManager& AudioManager::getInstance() {
    static AudioManager instance;
    return instance;
}

// Destructor
AudioManager::~AudioManager() {
    shutdown();
}

// Initialize audio system
bool AudioManager::initialize(const std::string& audioDirectory) {
    audioPath = audioDirectory;

#ifdef SFML_AVAILABLE
    std::cout << "[Audio] SFML audio initialized." << std::endl;
    std::cout << "[Audio] Audio path: " << audioPath << std::endl;
    return true;
#else
    std::cout << "[Audio] SFML not available - audio disabled." << std::endl;
    std::cout << "[Audio] To enable audio, compile with SFML and define SFML_AVAILABLE." << std::endl;
    return false;
#endif
}

// Shutdown audio system
void AudioManager::shutdown() {
#ifdef SFML_AVAILABLE
    stopMusic();
    stopAllSounds();

    // Clean up buffers
    for (int i = 0; i < 10; i++) {
        delete soundBuffers[i];
        soundBuffers[i] = nullptr;
    }

    for (int i = 0; i < 5; i++) {
        delete activeSounds[i];
        activeSounds[i] = nullptr;
    }
#endif
    std::cout << "[Audio] Audio system shutdown." << std::endl;
}

// Play background music
bool AudioManager::playMusic(const std::string& filename, bool loop) {
    (void)loop;  // Suppress unused parameter warning when SFML not available
    if (!audioEnabled || !musicEnabled) return false;

#ifdef SFML_AVAILABLE
    std::string fullPath = audioPath + "/" + filename;
    if (backgroundMusic.openFromFile(fullPath)) {
        backgroundMusic.setLoop(loop);
        backgroundMusic.setVolume(musicVolume * masterVolume * 100);
        backgroundMusic.play();
        currentMusicTrack = filename;
        std::cout << "[Audio] Playing music: " << filename << std::endl;
        return true;
    } else {
        std::cout << "[Audio] Failed to load music: " << fullPath << std::endl;
        return false;
    }
#else
    std::cout << "[Audio] Would play music: " << filename << std::endl;
    currentMusicTrack = filename;
    return true;
#endif
}

void AudioManager::stopMusic() {
#ifdef SFML_AVAILABLE
    backgroundMusic.stop();
#endif
    currentMusicTrack = "";
    std::cout << "[Audio] Music stopped." << std::endl;
}

void AudioManager::pauseMusic() {
#ifdef SFML_AVAILABLE
    backgroundMusic.pause();
#endif
    std::cout << "[Audio] Music paused." << std::endl;
}

void AudioManager::resumeMusic() {
#ifdef SFML_AVAILABLE
    backgroundMusic.play();
#endif
    std::cout << "[Audio] Music resumed." << std::endl;
}

bool AudioManager::isMusicPlaying() const {
#ifdef SFML_AVAILABLE
    return backgroundMusic.getStatus() == sf::Music::Playing;
#else
    return !currentMusicTrack.empty();
#endif
}

// Play sound effect
bool AudioManager::playSound(const std::string& filename) {
    if (!audioEnabled || !sfxEnabled) return false;

#ifdef SFML_AVAILABLE
    // Find available sound slot
    int slot = -1;
    for (int i = 0; i < 5; i++) {
        if (activeSounds[i] == nullptr ||
            activeSounds[i]->getStatus() == sf::Sound::Stopped) {
            slot = i;
            break;
        }
    }

    if (slot < 0) {
        std::cout << "[Audio] No available sound slots." << std::endl;
        return false;
    }

    // Load and play sound
    if (soundBuffers[slot] == nullptr) {
        soundBuffers[slot] = new sf::SoundBuffer();
    }

    std::string fullPath = audioPath + "/" + filename;
    if (soundBuffers[slot]->loadFromFile(fullPath)) {
        if (activeSounds[slot] == nullptr) {
            activeSounds[slot] = new sf::Sound();
        }
        activeSounds[slot]->setBuffer(*soundBuffers[slot]);
        activeSounds[slot]->setVolume(sfxVolume * masterVolume * 100);
        activeSounds[slot]->play();
        return true;
    } else {
        std::cout << "[Audio] Failed to load sound: " << fullPath << std::endl;
        return false;
    }
#else
    std::cout << "[Audio] *" << filename << "*" << std::endl;
    return true;
#endif
}

void AudioManager::stopAllSounds() {
#ifdef SFML_AVAILABLE
    for (int i = 0; i < 5; i++) {
        if (activeSounds[i] != nullptr) {
            activeSounds[i]->stop();
        }
    }
#endif
}

// Predefined sound effects
void AudioManager::playCombatStart() {
    playSound("combat_alert.wav");
    std::cout << "* Combat alert sound *" << std::endl;
}

void AudioManager::playDamageSound() {
    playSound("damage_taken.wav");
    std::cout << "* Damage sound *" << std::endl;
}

void AudioManager::playHealSound() {
    playSound("heal.wav");
    std::cout << "* Healing sound *" << std::endl;
}

void AudioManager::playLevelUpSound() {
    playSound("levelup.wav");
    std::cout << "* Level up fanfare *" << std::endl;
}

void AudioManager::playHackSuccess() {
    playSound("hack_success.wav");
    std::cout << "* Hack success sound *" << std::endl;
}

void AudioManager::playHackFail() {
    playSound("hack_fail.wav");
    std::cout << "* Hack failure sound *" << std::endl;
}

void AudioManager::playMenuSelect() {
    playSound("menu_select.wav");
}

void AudioManager::playMenuBack() {
    playSound("menu_back.wav");
}

void AudioManager::playEventTrigger() {
    playSound("event_trigger.wav");
    std::cout << "* Event trigger sound *" << std::endl;
}

void AudioManager::playVictorySound() {
    playSound("victory.wav");
    std::cout << "* Victory fanfare *" << std::endl;
}

void AudioManager::playDefeatSound() {
    playSound("defeat.wav");
    std::cout << "* Defeat sound *" << std::endl;
}

// Volume control
void AudioManager::setMasterVolume(float volume) {
    masterVolume = (volume < 0.0f) ? 0.0f : (volume > 1.0f) ? 1.0f : volume;
#ifdef SFML_AVAILABLE
    backgroundMusic.setVolume(musicVolume * masterVolume * 100);
#endif
}

void AudioManager::setMusicVolume(float volume) {
    musicVolume = (volume < 0.0f) ? 0.0f : (volume > 1.0f) ? 1.0f : volume;
#ifdef SFML_AVAILABLE
    backgroundMusic.setVolume(musicVolume * masterVolume * 100);
#endif
}

void AudioManager::setSFXVolume(float volume) {
    sfxVolume = (volume < 0.0f) ? 0.0f : (volume > 1.0f) ? 1.0f : volume;
}

float AudioManager::getMasterVolume() const { return masterVolume; }
float AudioManager::getMusicVolume() const { return musicVolume; }
float AudioManager::getSFXVolume() const { return sfxVolume; }

// Audio toggles
void AudioManager::enableAudio(bool enable) { audioEnabled = enable; }
void AudioManager::enableMusic(bool enable) { musicEnabled = enable; }
void AudioManager::enableSFX(bool enable) { sfxEnabled = enable; }

bool AudioManager::isAudioEnabled() const { return audioEnabled; }
bool AudioManager::isMusicEnabled() const { return musicEnabled; }
bool AudioManager::isSFXEnabled() const { return sfxEnabled; }

// Debug display
void AudioManager::displayStatus() const {
    std::cout << "\n=== AUDIO STATUS ===" << std::endl;
    std::cout << "Audio enabled: " << (audioEnabled ? "Yes" : "No") << std::endl;
    std::cout << "Music enabled: " << (musicEnabled ? "Yes" : "No") << std::endl;
    std::cout << "SFX enabled: " << (sfxEnabled ? "Yes" : "No") << std::endl;
    std::cout << "Master volume: " << (masterVolume * 100) << "%" << std::endl;
    std::cout << "Music volume: " << (musicVolume * 100) << "%" << std::endl;
    std::cout << "SFX volume: " << (sfxVolume * 100) << "%" << std::endl;
    std::cout << "Current track: " << (currentMusicTrack.empty() ? "None" : currentMusicTrack) << std::endl;
    std::cout << "===================" << std::endl;
}

/*******************************************************************************
 * CONVENIENCE FUNCTIONS
 ******************************************************************************/

void playBackgroundMusic(const std::string& track) {
    AudioManager::getInstance().playMusic(track);
}

void playSoundEffect(const std::string& sound) {
    AudioManager::getInstance().playSound(sound);
}

void stopAllAudio() {
    AudioManager::getInstance().stopMusic();
    AudioManager::getInstance().stopAllSounds();
}
