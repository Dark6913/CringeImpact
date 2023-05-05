#include "Enemy.hpp"
#include "VectorMath.hpp"
#include <random>

sf::SoundBuffer* Enemy::m_death_buffer = NULL;
sf::SoundBuffer* Enemy::m_walk_buffer = NULL;
uint32_t Enemy::m_instances_count = 0;

Enemy::Enemy()
{
	m_target = NULL;
	m_move_distance = 0;
	is_have_to_move = false;
	m_is_walk_sound_play = false;
	m_walk_timer = 0.f;
	m_walk_cd = 3.f;
	m_living_area_center = {0, 0};
	m_living_area_radius = 1.f;
	m_vision_angle = 0.f;
	m_speed = 300.f;
	m_attack_range = 50.f;
	m_attack_damage = 10.f;
	m_agressive_range = 256.f;
	m_living_area_extra_range = 500.f;
	m_calm_range = 0.f;
	m_escape_distance = m_agressive_range + 100.f;

	// Move animation
	m_move_anim.loadFromFile("data/textures/coal/Tleiushiy_tileset(walk).png", 4, 6, 0.2f);
	m_move_anim.setScale(sf::Vector2f(4, 4));
	m_move_anim.setOrigin(sf::Vector2f(.5f, .5f));
	m_move_anim.reset();
	m_move_anim.setCurrentRow(STAY_LEFT);
	m_last_move_dir = MOVE_LEFT;
	this->setCurrentAnimation(&m_move_anim);

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

	if (!m_walk_buffer)
	{
		m_walk_buffer = new sf::SoundBuffer();
		m_walk_buffer->loadFromFile("data/sound/walk_grass.ogg");
	}
	m_walk_sound.setBuffer(*m_walk_buffer);
	m_walk_sound.setVolume(20);
	m_walk_sound.setLoop(true);
	m_walk_sound_ptr = &m_walk_sound;

	// Hitboxes
	m_hitboxes_list.push_back(Hitbox(sf::Vector2f(44, 8), sf::Vector2f(32, 32), 2.f, this));
	m_hitboxes_list.push_back(Hitbox(sf::Vector2f(36, 40), sf::Vector2f(52, 88), 1.0f, this));

	m_instances_count++;
}

void Enemy::moveTo(sf::Vector2f point)
{
	is_have_to_move = true;
	m_move_distance = VectorModule(point - m_position);
	m_vision_angle = VectorArgument(point - m_position);

	switch (this->getVisionSector())
	{
	case WATCH_RIGHT:
		m_animation_ptr->setCurrentRow(MOVE_RIGHT);
		break;
	case WATCH_LEFT:
		m_animation_ptr->setCurrentRow(MOVE_LEFT);
		break;
	case WATCH_TOP:
		m_animation_ptr->setCurrentRow(MOVE_TOP);
		break;
	case WATCH_DOWN:
		m_animation_ptr->setCurrentRow(MOVE_BOTTOM);
		break;
	default: break;
	}

	if (m_vision_angle > M_PI_2 && m_vision_angle < M_3_PI_2) m_last_move_dir = MOVE_LEFT;
	else m_last_move_dir = MOVE_RIGHT;
}

void Enemy::behave(float tick, std::list<Entity*> players_list, std::list<Solid*>& solid_list)
{
	// Stop playing sound and return if dead
	if (m_is_dead)
	{
		if (m_is_walk_sound_play)
		{
			m_is_walk_sound_play = false;
			m_walk_sound.stop();
		}
		return;
	}

	if (!m_target)
	{
		for (auto it = players_list.begin(); it != players_list.end(); it++)
		{
			Entity* cur_player = *it;
			if (!cur_player->isDead() && VectorModule(cur_player->getPosition() - m_position) <= m_agressive_range)
			{
				m_target = cur_player;
				break;
			}
		}
	}

	// Random movement
	if (!m_target && m_walk_timer >= m_walk_cd)
	{
		this->moveTo(RandomVector(m_living_area_center, m_living_area_radius));
		m_walk_timer = 0.f;
	}
	else if (!m_target && !is_have_to_move) m_walk_timer += tick;

	// Attack traget
	if (m_target)
	{
		float distance = VectorModule(m_target->getPosition() - m_position);
		float distance_from_living_center = VectorModule(m_position - m_living_area_center);
		if (distance > m_attack_range && distance < m_escape_distance && distance_from_living_center < m_calm_range && !m_is_attacking)
		{
			this->moveTo(m_target->getPosition());
		}
		else if (distance >= m_escape_distance || distance_from_living_center >= m_calm_range)
		{
			m_walk_timer = m_walk_cd;
			m_target = NULL;
		}
		else if (distance <= m_attack_range)
		{
			this->stopMoving();
			attack(m_target->getPosition(), players_list);
		}

		if (m_target && m_target->isDead())
		{
			m_walk_timer = m_walk_cd;
			m_target = NULL;
		}
	}

	// Movement
	if (is_have_to_move)
	{
		sf::Vector2f delta_move = sf::Vector2f(m_speed * cos(m_vision_angle) * tick, m_speed * sin(m_vision_angle) * tick);
		for (auto it = solid_list.begin(); it != solid_list.end(); it++)
		{
			if (*it == this) continue;
			if (delta_move.x && this->isMovedIntersects(*it, sf::Vector2f(delta_move.x, 0.f))) delta_move.x = 0.f;
			if (delta_move.y && this->isMovedIntersects(*it, sf::Vector2f(0.f, delta_move.y))) delta_move.y = 0.f;
			if (delta_move.x && delta_move.y && this->isMovedIntersects(*it, delta_move)) delta_move = {0.f, 0.f};
			if (!delta_move.x && !delta_move.y)
			{
				m_move_distance = 0;
				break;
			}
		}

		this->move(delta_move);
		m_move_distance -= VectorModule(delta_move);

		float walk_sound_volume = 20.f * (1.f - VectorModule(m_listener_position - m_position) / 1000.f);
		if (walk_sound_volume < 0) walk_sound_volume = 0;
		m_walk_sound.setVolume(walk_sound_volume);
		if (!m_is_walk_sound_play)
		{
			m_walk_sound.play();
			m_is_walk_sound_play = true;
		}

		if (m_move_distance <= 0) stopMoving();
	}
}

void Enemy::setLivingArea(sf::Vector2f center, float radius)
{
	m_living_area_center = center;
	m_living_area_radius = radius;
	m_calm_range = radius + m_living_area_extra_range;
}

void Enemy::stopMoving()
{
	if (is_have_to_move)
	{
		if (m_is_walk_sound_play)
		{
			m_walk_sound.stop();
			m_is_walk_sound_play = false;
		}

		m_move_distance = 0.f;
		is_have_to_move = false;
		m_animation_ptr->setCurrentRow(m_last_move_dir + 3);
	}
}

void Enemy::setTarget(Entity* target)
{
	m_target = target;
}

Enemy::~Enemy()
{
	m_instances_count--;
	if (m_instances_count == 0)
	{
		if (m_death_buffer) delete m_death_buffer;
		if (m_walk_buffer) delete m_walk_buffer;
	}
}