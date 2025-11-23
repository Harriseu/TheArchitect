#include "../include/Audio.h"

#ifdef SFML_AVAILABLE
#include <SFML/System.hpp>
#endif

AudioManager::AudioManager()
    : audioEnabled(true), musicEnabled(true), sfxEnabled(true),
    masterVolume(1.0f), musicVolume(0.7f), sfxVolume(0.8f),
    currentMusicTrack(""), audioPath("") {
}

AudioManager& AudioManager::getInstance() {
    static AudioManager instance;
    return instance;
}

AudioManager::~AudioManager() {
    shutdown();
}

bool AudioManager::initialize(const std::string& audioDirectory) {
    audioPath = audioDirectory;

#ifdef SFML_AVAILABLE
    std::cout << "[Audio] SFML (v3.0) audio initialized." << std::endl; // Updated version
    std::cout << "[Audio] Audio path: " << audioPath << std::endl;
    return true;
#else
    std::cout << "[Audio] SFML not available - audio disabled." << std::endl;
    return false;
#endif
}

void AudioManager::shutdown() {
#ifdef SFML_AVAILABLE
    stopMusic();
    stopAllSounds();
    soundBufferCache.clear();
#endif
    std::cout << "[Audio] Audio system shutdown." << std::endl;
}

bool AudioManager::playMusic(const std::string& filename, bool loop) {
    if (!audioEnabled || !musicEnabled) return false;

#ifdef SFML_AVAILABLE
    std::string fullPath = audioPath + "/" + filename;
    if (backgroundMusic.openFromFile(fullPath)) {

        // SFML 3 CHANGE: setLoop is now setLooping
        backgroundMusic.setLooping(loop);

        backgroundMusic.setVolume(musicVolume * masterVolume * 100.f);
        backgroundMusic.play();
        currentMusicTrack = filename;
        std::cout << "[Audio] Playing music: " << filename << std::endl;
        return true;
    }
    else {
        std::cout << "[Audio] Failed to load music: " << fullPath << std::endl;
        return false;
    }
#else
    (void)loop; // Suppress unused parameter warning
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
    // SFML 3 CHANGE: Unscoped enum sf::SoundSource::Playing is now sf::SoundSource::Status::Playing
    return backgroundMusic.getStatus() == sf::SoundSource::Status::Playing;
#else
    return !currentMusicTrack.empty();
#endif
}

void AudioManager::cleanupFinishedSounds() {
#ifdef SFML_AVAILABLE
    // Remove sounds from the active list that have finished playing
    activeSounds.remove_if([](const sf::Sound& s) {
        // SFML 3 CHANGE: Use scoped enum sf::SoundSource::Status::Stopped
        return s.getStatus() == sf::SoundSource::Status::Stopped;
        });
#endif
}

bool AudioManager::playSound(const std::string& filename) {
    if (!audioEnabled || !sfxEnabled) return false;

#ifdef SFML_AVAILABLE
    // First, clean up any sounds that have finished
    cleanupFinishedSounds();

    // Optional: Limit the number of concurrent sounds
    if (activeSounds.size() >= MAX_CONCURRENT_SOUNDS) {
        std::cout << "[Audio] Too many sounds playing. " << filename << " skipped." << std::endl;
        return false;
    }

    // Check if the buffer is already in our cache
    if (soundBufferCache.find(filename) == soundBufferCache.end()) {
        // If not, load it and add it to the cache
        std::string fullPath = audioPath + "/" + filename;
        sf::SoundBuffer buffer;
        if (buffer.loadFromFile(fullPath)) {
            soundBufferCache[filename] = buffer;
            std::cout << "[Audio] Loaded and cached sound: " << filename << std::endl;
        }
        else {
            std::cout << "[Audio] Failed to load sound: " << fullPath << std::endl;
            return false;
        }
    }

    // SFML 3 CHANGE: Create a sound on the fly, as sf::Sound() is deleted.
    // emplace_back constructs the Sound in place, using the cached buffer.
    activeSounds.emplace_back(soundBufferCache[filename]);
    sf::Sound& sound = activeSounds.back(); // Get a reference to the new sound

    sound.setVolume(sfxVolume * masterVolume * 100.f);
    sound.play();

    return true;
#else
    std::cout << "[Audio] *SFX: " << filename << "*" << std::endl;
    return true;
#endif
}

void AudioManager::stopAllSounds() {
#ifdef SFML_AVAILABLE
    // Simply clearing the list stops and destroys all sound instances
    activeSounds.clear();
#endif
}


// Convenience wrappers
void AudioManager::playCombatStart() { playSound("combat_alert.wav"); }
void AudioManager::playDamageSound() { playSound("damage_taken.wav"); }
void AudioManager::playHealSound() { playSound("heal.wav"); }
void AudioManager::playLevelUpSound() { playSound("levelup.wav"); }
void AudioManager::playHackSuccess() { playSound("hack_success.wav"); }
void AudioManager::playHackFail() { playSound("hack_fail.wav"); }
void AudioManager::playMenuSelect() { playSound("menu_select.wav"); }
void AudioManager::playMenuBack() { playSound("menu_back.wav"); }
void AudioManager::playEventTrigger() { playSound("event_trigger.wav"); }
void AudioManager::playVictorySound() { playSound("victory.wav"); }
void AudioManager::playDefeatSound() { playSound("defeat.wav"); }

void AudioManager::setMasterVolume(float volume) {
    masterVolume = std::max(0.0f, std::min(1.0f, volume));
#ifdef SFML_AVAILABLE
    backgroundMusic.setVolume(musicVolume * masterVolume * 100.f);
#endif
}

void AudioManager::setMusicVolume(float volume) {
    musicVolume = std::max(0.0f, std::min(1.0f, volume));
#ifdef SFML_AVAILABLE
    backgroundMusic.setVolume(musicVolume * masterVolume * 100.f);
#endif
}

void AudioManager::setSFXVolume(float volume) {
    sfxVolume = std::max(0.0f, std::min(1.0f, volume));
}

float AudioManager::getMasterVolume() const { return masterVolume; }
float AudioManager::getMusicVolume() const { return musicVolume; }
float AudioManager::getSFXVolume() const { return sfxVolume; }

void AudioManager::enableAudio(bool enable) { audioEnabled = enable; }
void AudioManager::enableMusic(bool enable) { musicEnabled = enable; }
void AudioManager::enableSFX(bool enable) { sfxEnabled = enable; }

bool AudioManager::isAudioEnabled() const { return audioEnabled; }
bool AudioManager::isMusicEnabled() const { return musicEnabled; }
bool AudioManager::isSFXEnabled() const { return sfxEnabled; }

void AudioManager::displayStatus() const {
    std::cout << "\n=== AUDIO STATUS ===" << std::endl;
    std::cout << "Audio enabled: " << (audioEnabled ? "Yes" : "No") << std::endl;
    std::cout << "Music enabled: " << (musicEnabled ? "Yes" : "No") << std::endl;
    std::cout << "SFX enabled: " << (sfxEnabled ? "Yes" : "No") << std::endl;
    std::cout << "Master volume: " << (masterVolume * 100) << "%" << std::endl;
    std::cout << "Music volume: " << (musicVolume * 100) << "%" << std::endl;
    std::cout << "SFX volume: " << (sfxVolume * 100) << "%" << std::endl;
    std::cout << "Current track: " << (currentMusicTrack.empty() ? "None" : currentMusicTrack) << std::endl;
#ifdef SFML_AVAILABLE
    std::cout << "Active SFX: " << activeSounds.size() << std::endl;
#endif
    std::cout << "===================" << std::endl;
}

// Global convenience function implementations
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