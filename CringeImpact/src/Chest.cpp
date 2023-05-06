#include "Chest.hpp"

sf::SoundBuffer* Chest::m_sound_buffer = NULL;
uint32_t Chest::m_instances_count = 0;

Chest::Chest()
{
	m_animation_ptr = &m_animation;
	m_is_open = false;
	m_was_open = false;
	m_animation_ptr->reset();
	if (!m_sound_buffer)
	{
		m_sound_buffer = new sf::SoundBuffer();
		m_sound_buffer->loadFromFile("data/audio/sound/chest-barrel-open.ogg");
	}
	m_instances_count++;
	m_sound.setBuffer(*m_sound_buffer);
}

void Chest::open()
{
	if (!m_was_open)
	{
		m_sound.play();
		m_is_open = true;
	}
}

void Chest::close()
{
	if (m_was_open)
	{
		m_sound.play();
		m_is_open = false;
	}
}

void Chest::toggle()
{
	if (!m_was_open)
	{
		m_sound.play();
		m_is_open = true;
	}
	else if (m_was_open)
	{
		m_sound.play();
		m_is_open = false;
	}
}

void Chest::update(float tick)
{
	if (!m_was_open && m_is_open && this->m_animation_ptr->playInStraightOrder(tick)) m_was_open = m_is_open;
	else if (m_was_open && !m_is_open && this->m_animation_ptr->playInReverseOrder(tick)) m_was_open = m_is_open;
	float volume = 40.f * (1.f - VectorModule(m_listener_position - this->getCenter()) / 1000.f);
	if (volume <= 0.f) volume = 0.f;
	m_sound.setVolume(volume);
}

Chest::~Chest()
{
	m_instances_count--;
	if (m_instances_count == 0) delete m_sound_buffer;
}