#include "Player.hpp"
#include "SoundRegister.hpp"

Player::Player()
{
	// Move animation
	m_move_anim.loadFromFile("data/textures/Hina/Hina-Move.png", 4, 6, 0.2f);
	m_move_anim.setScale(sf::Vector2f(4.f, 4.f));
	m_move_anim.setOrigin(sf::Vector2f(.5f, .5f));
	m_move_anim.reset();
	m_move_anim.setCurrentRow(STAY_RIGHT);
	this->setCurrentAnimation(&m_move_anim);

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

	m_collisions.push_back(sf::IntRect(24, 120, 32, 8));
	this->setPosition(m_position);

	m_is_walk_sound_plays = false;
	m_is_attacking = false;

	m_speed = 300.f;
	m_attack_range = 80.f;
	m_attack_damage = 25.f;

	// Hitboxes
	m_hitboxes_list.push_back(Hitbox(sf::Vector2f(20, 4), sf::Vector2f(40, 40), 2.f, this));
	m_hitboxes_list.push_back(Hitbox(sf::Vector2f(16, 44), sf::Vector2f(48, 84), 1.0f, this));

	// Sound
	m_walk_sound_ptr = SoundRegister::createSound("player-walk-grass", 20.f, true);
	m_hit_sound_ptr = SoundRegister::createSound("player-hit", 20.f);
	m_hurt_sound_ptr = SoundRegister::createSound("player-hurt", 20.f);
	m_death_sound_ptr = SoundRegister::createSound("player-death", 20.f);
}

void Player::control(float tick, std::list<Solid*> solid_list)
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
			if (m_walk_sound_ptr) m_walk_sound_ptr->play();
			m_is_walk_sound_plays = true;
		}

		for (auto it = solid_list.begin(); it != solid_list.end(); it++)
		{
			if (*it == this) continue;
			if (delta_move.x && this->isMovedIntersects(*it, sf::Vector2f(delta_move.x, 0))) delta_move.x = 0;
			if (delta_move.y && this->isMovedIntersects(*it, sf::Vector2f(0, delta_move.y))) delta_move.y = 0;
			if (delta_move.x && delta_move.y && this->isMovedIntersects(*it, delta_move)) delta_move = { 0.f, 0.f };
			if (!delta_move.x && !delta_move.y) break;

		}
		this->move(delta_move);
	}
	else if (!m_is_dead)
	{
		if (m_is_walk_sound_plays)
		{
			if (m_walk_sound_ptr) m_walk_sound_ptr->pause();
			m_is_walk_sound_plays = false;
		}

		if (!m_is_attacking) m_animation_ptr->setCurrentRow(m_last_move_dir + 3);
	}
}