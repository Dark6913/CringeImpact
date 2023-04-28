#include "Cursor.hpp"

Cursor::Cursor()
{
	m_cursor_size = {32u, 32u};
	m_sprite.setTextureRect(sf::IntRect(0, 0, m_cursor_size.x, m_cursor_size.y));
}

void Cursor::loadFromFile(std::string file)
{
	m_texture.loadFromFile(file);
	m_sprite.setTexture(m_texture);
}

void Cursor::setPosition(sf::Vector2f position)
{
	m_sprite.setPosition(position);
}

void Cursor::setCursorStyle(CursorStyle style)
{
	m_sprite.setTextureRect(sf::IntRect(style * m_cursor_size.x, 0, m_cursor_size.x, m_cursor_size.y));
}

void Cursor::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(m_sprite, states);
}