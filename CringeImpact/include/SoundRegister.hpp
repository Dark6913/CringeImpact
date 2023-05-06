#pragma once
#include <SFML/Audio.hpp>
#include <string>
#include <filesystem>
#include <iostream>
#include <unordered_map>
#include <queue>

class SoundRegister
{
public:
	// Load all OGG files to sound buffer map
	static void loadAllFromDirectory(std::string path);

	// Returns a pointer to pointer to created sound instance with given name
	static sf::Sound* createSound(std::string name, int volume = 100, bool loop = false);

	// Returns a pointer to pointer to created music opened from file
	static sf::Music* createMusic(std::string path, int volume = 100, bool loop = false);

	// Remove sound source from queue
	static void remove(sf::SoundSource* source);

	// Remove sound buffer from map by given name
	static void remove(std::string name);

	// Calls clearBufferMap() and clearSoundSourceQueue()
	static void clear();

	// Clear sound buffer map and relese all allocated memory for sound buffers
	static void clearBufferMap();

	// Clear sound source queue and relese all allocated memory for sound source
	static void clearSoundSourceQueue();

	// Check is sound source existing
	static bool isSoundSourceExisting(sf::SoundSource* source);
private:
	// Private constructor to avoid creating an instances
	SoundRegister() {};

	// Check whether sound sources amount less than max sources amount
	// If it's true just increace source amount counter
	// If it's false calls deletSource() for first element in sound source queue
	static void onCreate();

	// Release allocated memory for sound source
	static void deleteSource(sf::SoundSource* source);

	// Add sound source to sounds queue and sounds set
	static void registerSource(sf::SoundSource* source);

	static std::string m_path;
	static std::unordered_map<std::string, sf::SoundBuffer*> m_buffer_map;
	static std::queue<sf::SoundSource*> m_sounds_queue;
	static std::set<sf::SoundSource*> m_sounds_set; // Sound source set to check if source existing
	static const int m_max_sources_amount;
};