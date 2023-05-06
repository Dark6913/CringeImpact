#include "SoundRegister.hpp"

std::string SoundRegister::m_path = "";
std::unordered_map<std::string, sf::SoundBuffer*> SoundRegister::m_buffer_map;
const int SoundRegister::m_max_sources_amount = 256; // OS restriction
std::queue<sf::SoundSource*> SoundRegister::m_sounds_queue;
std::set<sf::SoundSource*> SoundRegister::m_sounds_set;

void SoundRegister::loadAllFromDirectory(std::string path)
{
	SoundRegister::clearBufferMap();
	if (!(path.back() == '/' || path.back() == '\\')) path += '/';
	try
	{
		for (const auto& entry : std::filesystem::directory_iterator(path))
		{
			if (entry.is_regular_file() && entry.path().extension().string() == ".ogg")
			{
				std::string file_name = entry.path().filename().string();
				sf::SoundBuffer* buffer = new sf::SoundBuffer();
				buffer->loadFromFile(path  + file_name);
				std::string str_key = file_name.substr(0, file_name.rfind('.'));
				m_buffer_map.insert(std::pair(str_key, buffer));
			}
		}
	}
	catch (std::exception& e)
	{
		std::cout << "An error occured while loading sound buffer: " << e.what() << std::endl;
	}
}

sf::Sound* SoundRegister::createSound(std::string name)
{
	SoundRegister::onCreate();
	sf::Sound* sound = new sf::Sound();
	sound->setBuffer(*m_buffer_map.at(name));
	SoundRegister::registerSource(sound);
	return sound;
}

sf::Music* SoundRegister::createMusic(std::string path)
{
	SoundRegister::onCreate();
	sf::Music* music = new sf::Music();
	music->openFromFile(path);
	SoundRegister::registerSource(music);
	return music;
}

void SoundRegister::onCreate()
{
	if (m_sounds_queue.size() == m_max_sources_amount)
	{
		SoundRegister::deleteSource(m_sounds_queue.front());
		m_sounds_queue.pop();
	}
}

void SoundRegister::remove(sf::SoundSource* source)
{
	std::queue<sf::SoundSource*> buffer_queue;
	while (!m_sounds_queue.empty())
	{
		if (m_sounds_queue.front() != source) buffer_queue.push(m_sounds_queue.front());
		else SoundRegister::deleteSource(m_sounds_queue.front());
		m_sounds_queue.pop();
	}
	m_sounds_queue = buffer_queue;
}

void SoundRegister::remove(std::string name)
{
	m_buffer_map.erase(name);
}

void SoundRegister::clear()
{
	SoundRegister::clearBufferMap();
	SoundRegister::clearSoundSourceQueue();
}

void SoundRegister::clearBufferMap()
{
	for (auto& el : m_buffer_map)
		delete el.second;
	m_buffer_map.clear();
}

void SoundRegister::clearSoundSourceQueue()
{
	while (!m_sounds_queue.empty())
	{
		delete m_sounds_queue.front();
		m_sounds_queue.pop();
	}
}

bool SoundRegister::isSoundSourceExisting(sf::SoundSource* source)
{
	return m_sounds_set.find(source) != m_sounds_set.end();
}

void SoundRegister::deleteSource(sf::SoundSource* source)
{
	delete source;
	m_sounds_set.erase(source);
}

void SoundRegister::registerSource(sf::SoundSource* source)
{
	m_sounds_queue.push(source);
	m_sounds_set.insert(source);
}