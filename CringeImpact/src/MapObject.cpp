#include <list>
#include "MapObject.hpp"
#include "VectorMath.hpp"

MapObject::MapObject()
{
	this->setPosition(m_position);
	m_animation_ptr = NULL;
}

void MapObject::setPosition(sf::Vector2f position)
{
	m_position = position;
	this->setAbsoluteObjectPosition(m_position);
	if (m_animation_ptr) m_animation_ptr->setPosition(m_position);
}

void MapObject::loadAnimation(sf::Texture& texture, uint8_t frames_count, uint8_t rows_count,
	float frame_duration, sf::Vector2f scale, sf::Vector2f origin)
{
	if (m_animation_ptr)
	{
		m_animation_ptr->loadFromMemory(texture, frames_count, rows_count, frame_duration);
		m_animation_ptr->setScale(scale);
		m_animation_ptr->setOrigin(origin);
		this->setObjectTextureOriginOffset(
			sf::Vector2f(
				m_animation_ptr->getOriginOffset().x * scale.x,
				m_animation_ptr->getOriginOffset().y * scale.y
			)
		);
	}
}

void MapObject::loadAnimation(std::string file, uint8_t frames_count, uint8_t rows_count,
	float frame_duration, sf::Vector2f scale, sf::Vector2f origin)
{
	if (m_animation_ptr)
	{
		m_animation_ptr->loadFromFile(file, frames_count, rows_count, frame_duration);
		m_animation_ptr->setScale(scale);
		m_animation_ptr->setOrigin(origin);
		this->setObjectTextureOriginOffset(
			sf::Vector2f(
				m_animation_ptr->getOriginOffset().x * scale.x,
				m_animation_ptr->getOriginOffset().y * scale.y
			)
		);
	}
}

void MapObject::addCollisionRect(sf::IntRect rect)
{
	m_collisions.push_back(rect);
}

sf::Vector2f MapObject::getPosition()
{
	return m_position;
}

sf::FloatRect MapObject::getVisibleBounds() const
{
	return m_animation_ptr->getGlobalBounds();
}

sf::Vector2f MapObject::getCenter() const
{
	if (m_animation_ptr)
	{
		return m_position - (m_animation_ptr->getOriginOffset() * m_animation_ptr->getScale()) +
			sf::Vector2f(this->getVisibleBounds().width / 2.f, this->getVisibleBounds().height / 2.f);
	}
	else return m_position;
}

void MapObject::update(float tick)
{
	if (m_animation_ptr)
	{
		m_animation_ptr->playLoopInStraightOrder(tick);
	}
}

void MapObject::setCurrentAnimation(Animation* anim)
{
	m_animation_ptr = anim;
	if (anim) this->setObjectTextureOriginOffset(anim->getOriginOffset() * anim->getScale());
}

void MapObject::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (m_animation_ptr) target.draw(*m_animation_ptr, states);
}