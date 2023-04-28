#include "Enemy.hpp"

sf::SoundBuffer* Enemy::m_death_buffer = NULL;
uint32_t Enemy::m_instances_count = 0;

Enemy::Enemy()
{
	// Move animation
	m_animation_ptr = &m_move_anim;
	m_move_anim.loadFromFile("data/textures/coal/Tleiushiy_tileset(walk).png", 4, 6, 0.2f);
	m_move_anim.setScale(sf::Vector2f(4, 4));
	m_move_anim.setOrigin(sf::Vector2f(.5f, .5f));
	m_move_anim.reset();
	m_move_anim.setCurrentRow(STAY_LEFT);
	m_last_move_dir = MOVE_LEFT;
	m_vision_angle = 3.14;

	// Attack animation
	m_attack_anim.loadFromFile("data/textures/coal/Tleiushiy_tileset(attack).png", 4, 4, 0.2f);
	m_attack_anim.setScale(sf::Vector2f(4.f, 4.f));
	m_attack_anim.setOrigin(sf::Vector2f(.5f, .5f));
	m_attack_anim.reset();

	// Die animation
	m_die_anim.loadFromFile("data/textures/coal/Tleiushiy_tileset(dead).png", 4, 2, 0.2f);
	m_die_anim.setScale(sf::Vector2f(4.f, 4.f));
	m_die_anim.setOrigin(sf::Vector2f(.5f, .5f));
	m_die_anim.reset();

	this->setObjectTextureOriginOffset(m_animation_ptr->getOriginOffset() * 4.f);
	m_collisions.push_back(sf::IntRect(48, 120, 28, 8));
	this->setPosition(m_position);

	if (!m_death_buffer)
	{
		m_death_buffer = new sf::SoundBuffer();
		m_death_buffer->loadFromFile("data/sound/coal_death_sound.ogg");
	}
	m_death_sound.setBuffer(*m_death_buffer);
	m_death_sound.setVolume(100);
	m_death_sound.setLoop(false);
	m_death_sound_ptr = &m_death_sound;
	m_instances_count++;
}

Enemy::~Enemy()
{
	m_instances_count--;
	if (m_instances_count == 0)
	{
		delete m_death_buffer;
	}
}