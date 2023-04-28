#include "Player.hpp"

sf::SoundBuffer* Player::m_walk_buffer = NULL;
sf::SoundBuffer* Player::m_attack_buffer = NULL;
uint32_t Player::m_instances_count = 0;

Player::Player()
{
	// Move animation
	m_animation_ptr = &m_move_anim;
	m_move_anim.loadFromFile("data/textures/Hina/Hina-Move.png", 4, 6, 0.2f);
	m_move_anim.setScale(sf::Vector2f(4.f, 4.f));
	m_move_anim.setOrigin(sf::Vector2f(.5f, .5f));
	m_move_anim.reset();
	m_move_anim.setCurrentRow(STAY_RIGHT);

	// Attack animation
	m_attack_anim.loadFromFile("data/textures/Hina/Hina-Attack.png", 4, 4, 0.2f);
	m_attack_anim.setScale(sf::Vector2f(4.f, 4.f));
	m_attack_anim.setOrigin(sf::Vector2f(.5f, .5f));
	m_attack_anim.reset();

	// Die animation
	m_die_anim.loadFromFile("data/textures/Hina/Hina-Death.png", 4, 2, 0.2f);
	m_die_anim.setScale(sf::Vector2f(4.f, 4.f));
	m_die_anim.setOrigin(sf::Vector2f(.5f, .5f));
	m_die_anim.reset();

	this->setObjectTextureOriginOffset(m_animation_ptr->getOriginOffset() * 4.f);
	m_collisions.push_back(sf::IntRect(24, 120, 32, 8));
	m_speed = 300.f;
	this->setPosition(m_position);

	if (!m_walk_buffer)
	{
		m_walk_buffer = new sf::SoundBuffer();
		m_walk_buffer->loadFromFile("data/sound/walk_grass.ogg");
		m_walk_sound.setBuffer(*m_walk_buffer);
		m_walk_sound.setVolume(20);
		m_walk_sound.setLoop(true);
	}

	if (!m_attack_buffer)
	{
		m_attack_buffer = new sf::SoundBuffer();
		m_attack_buffer->loadFromFile("data/sound/hit.ogg");
		m_attack_sound.setBuffer(*m_attack_buffer);
		m_attack_sound.setVolume(20);
		m_attack_sound.setLoop(false);
		m_attack_sound_ptr = &m_attack_sound;
	}

	m_instances_count++;
	m_is_walk_sound_plays = false;
	m_is_attacking = false;
}

void Player::control(float tick, sf::Vector2f mouse_pos, std::list<Solid*> solid_list)
{
	bool is_key_pressed = false;
	float factor = m_speed * tick;
	Entity::Movement move_dir = STAY_RIGHT;
	sf::Vector2f delta_move = { 0, 0 };
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		delta_move.y -= factor;
		move_dir = MOVE_TOP;
		is_key_pressed = true;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		delta_move.y += factor;
		move_dir = MOVE_BOTTOM;
		is_key_pressed = true;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		delta_move.x -= factor;
		move_dir = MOVE_LEFT;
		m_last_move_dir = MOVE_LEFT;
		is_key_pressed = true;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		delta_move.x += factor;
		move_dir = MOVE_RIGHT;
		m_last_move_dir = MOVE_RIGHT;
		is_key_pressed = true;
	}

	if (is_key_pressed && !m_is_dead)
	{
		if (!m_is_attacking) m_animation_ptr->setCurrentRow(move_dir);

		if (!m_is_walk_sound_plays)
		{
			m_walk_sound.play();
			m_is_walk_sound_plays = true;
		}

		for (auto it = solid_list.begin(); it != solid_list.end(); it++)
		{
			if (delta_move.x && this->isMovedIntersects(*it, sf::Vector2f(delta_move.x, 0))) delta_move.x = 0;
			if (delta_move.y && this->isMovedIntersects(*it, sf::Vector2f(0, delta_move.y))) delta_move.y = 0;

		}
		this->move(delta_move);
	}
	else if (!m_is_dead)
	{
		if (m_is_walk_sound_plays)
		{
			m_walk_sound.pause();
			m_is_walk_sound_plays = false;
		}

		if (!m_is_attacking) m_animation_ptr->setCurrentRow(m_last_move_dir + 3);
	}

	m_vision_angle = VectorArgument(mouse_pos - m_position);
}

Player::~Player()
{
	m_instances_count--;
	if (m_instances_count == 0)
	{
		delete m_walk_buffer;
		delete m_attack_buffer;
	}
}