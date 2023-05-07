#include "Chest.hpp"
#include "SoundRegister.hpp"

Chest::Chest()
{
	m_animation_ptr = &m_animation;
	m_is_open = false;
	m_was_open = false;
	m_animation_ptr->reset();

	m_open_sound_ptr = NULL;
	m_close_sound_ptr = NULL;
}

void Chest::open()
{
	if (!m_was_open)
	{
		if (m_open_sound_ptr) m_open_sound_ptr->play();
		m_is_open = true;
	}
}

void Chest::close()
{
	if (m_was_open)
	{
		if (m_close_sound_ptr) m_close_sound_ptr->play();
		m_is_open = false;
	}
}

void Chest::toggle()
{
	if (!m_was_open)
	{
		if (m_open_sound_ptr) m_open_sound_ptr->play();
		m_is_open = true;
	}
	else if (m_was_open)
	{
		if (m_close_sound_ptr) m_close_sound_ptr->play();
		m_is_open = false;
	}
}

void Chest::update(float tick)
{
	if (!m_was_open && m_is_open && this->m_animation_ptr->playInStraightOrder(tick)) m_was_open = m_is_open;
	else if (m_was_open && !m_is_open && this->m_animation_ptr->playInReverseOrder(tick)) m_was_open = m_is_open;
}