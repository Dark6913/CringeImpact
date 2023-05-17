#include "Entity.hpp"
#include "VectorMath.hpp"
#include "SoundRegister.hpp"

sf::Shader Entity::m_damaged_shader;
bool Entity::m_is_damaged_shader_loaded = false;

Entity::Entity()
{
	m_max_hp = 100;
	m_max_mp = 100;
	m_max_stamina = 100;
	m_current_hp = 100;
	m_current_mp = 100;
	m_current_stamina = 100;
	m_speed = 0;
	m_last_move_dir = MOVE_RIGHT;
	m_is_attacking = false;
	m_vision_angle = 0;
	m_is_attack_sound_playing = false;
	m_is_dead = false;
	m_is_damaged_recently = false;
	m_current_shader_ptr = NULL;
	m_damaged_timer = 0.f;
	m_attack_range = 0.f;
	m_attack_damage = 0.f;
	m_is_ready_to_respawn = false;

	m_walk_sound_ptr = NULL;
	m_death_sound_ptr = NULL;
	m_hit_sound_ptr = NULL;
	m_hurt_sound_ptr = NULL;

	if (!m_is_damaged_shader_loaded)
	{
		m_damaged_shader.loadFromFile("data/shaders/damaged.frag", sf::Shader::Fragment);
		m_is_damaged_shader_loaded = true;
	}
}

Entity::~Entity()
{
	if (SoundRegister::isSourceExisting(m_walk_sound_ptr)) SoundRegister::remove(m_walk_sound_ptr);
	if (SoundRegister::isSourceExisting(m_death_sound_ptr)) SoundRegister::remove(m_death_sound_ptr);
	if (SoundRegister::isSourceExisting(m_hit_sound_ptr)) SoundRegister::remove(m_hit_sound_ptr);
	if (SoundRegister::isSourceExisting(m_hurt_sound_ptr)) SoundRegister::remove(m_hurt_sound_ptr);
}

Hitbox* Entity::getAttackedHitbox(sf::Vector2f attack_point)
{
	for (auto it = m_hitboxes_list.begin(); it != m_hitboxes_list.end(); it++)
	{
		if (it->isContain(attack_point)) return &*it;
	}
	return NULL;
}

void Entity::move(sf::Vector2f delta)
{
	this->setPosition(m_position + delta);
}

void Entity::attack(sf::Vector2f attack_point, std::list<Entity*>& entitys_list)
{
	m_vision_angle = VectorArgument(attack_point - m_position);
	if (!m_is_attacking && !m_is_dead)
	{
		this->setCurrentAnimation(&m_attack_anim);
		m_animation_ptr->reset();
		m_animation_ptr->setPosition(m_position);
		
		switch (this->getVisionSector())
		{
		case WATCH_RIGHT:
			m_animation_ptr->setCurrentRow(ATTACK_RIGHT);
			break;
		case WATCH_LEFT:
			m_animation_ptr->setCurrentRow(ATTACK_LEFT);
			break;
		case WATCH_TOP:
			m_animation_ptr->setCurrentRow(ATTACK_TOP);
			break;
		case WATCH_DOWN:
			m_animation_ptr->setCurrentRow(ATTACK_BOTTOM);
			break;
		default: break;
		}

		if (m_vision_angle >= M_3_PI_2 || m_vision_angle <= M_PI_2) m_last_move_dir = MOVE_RIGHT;
		else m_last_move_dir = MOVE_LEFT;

		float attack_distance = VectorModule(attack_point - m_position);
		attack_distance = (attack_distance > m_attack_range) ? m_attack_range : attack_distance;
		sf::Vector2f actually_attack_point = m_position + attack_distance * sf::Vector2f(cos(m_vision_angle), sin(m_vision_angle));

		for (auto it = entitys_list.begin(); it != entitys_list.end(); it++)
		{
			Entity& enemy = **it;
			Hitbox* attacked_hitbox = enemy.getAttackedHitbox(actually_attack_point);
			if (attacked_hitbox)
			{
				enemy.takeDamage(m_attack_damage * attacked_hitbox->getDamageMultiply());
			}
		}

		m_is_attacking = true;
	}
}

void Entity::die()
{
	if (!m_is_dead)
	{
		this->setCurrentAnimation(&m_die_anim);
		m_animation_ptr->setCurrentRow(m_last_move_dir - 1);
		m_animation_ptr->setPosition(m_position);
		m_animation_ptr->reset();
		m_is_dead = true;
		if (m_death_sound_ptr) m_death_sound_ptr->play();
		if (m_walk_sound_ptr) m_walk_sound_ptr->stop();
	}
}

void Entity::respawn(sf::Vector2f position)
{
	if (m_is_ready_to_respawn)
	{
		m_is_dead = false;
		this->setCurrentAnimation(&m_move_anim);
		m_current_hp = m_max_hp;
		this->setPosition(position);
		m_is_ready_to_respawn = false;
	}
}

void Entity::takeDamage(float damage)
{
	if (!m_is_dead)
	{
		if (m_hurt_sound_ptr) m_hurt_sound_ptr->play();
		m_is_damaged_recently = true;
		m_current_shader_ptr = &m_damaged_shader;
		m_current_hp -= damage;
		if (m_current_hp <= 0.f)
		{
			m_current_hp = 0.f;
			this->die();
		}
	}
}

bool Entity::isDead()
{
	return m_is_dead;
}

void Entity::update(float tick)
{
	if (m_is_damaged_recently)
	{
		m_damaged_timer += tick;
		if (m_damaged_timer >= 0.2f)
		{
			m_is_damaged_recently = false;
			m_current_shader_ptr = NULL;
			m_damaged_timer = 0.f;
		}
	}

	if (m_animation_ptr)
	{
		if (!m_is_attacking && !m_is_dead) m_animation_ptr->playLoopInStraightOrder(tick);
		else if (m_is_attacking && !m_is_dead)
		{
			if (!m_is_attack_sound_playing && m_hit_sound_ptr)
			{
				m_hit_sound_ptr->play();
				m_is_attack_sound_playing = true;
			}

			if (m_animation_ptr->playInStraightOrder(tick))
			{
				m_is_attacking = false;
				this->setCurrentAnimation(&m_move_anim);
				m_animation_ptr->reset();
				m_animation_ptr->setPosition(m_position);
				m_is_attack_sound_playing = false;
			}
		}
		else if (m_is_dead)
		{
			if (!m_is_ready_to_respawn && m_animation_ptr->playInStraightOrder(tick))
			{
				m_is_ready_to_respawn = true;
			}
		}
	}
}

Entity::VisionDir Entity::getVisionSector()
{
	if (m_vision_angle >= M_7_PI_4 || m_vision_angle <= M_PI_4) return WATCH_RIGHT;
	else if (m_vision_angle > M_PI_4 && m_vision_angle < M_3_PI_4) return WATCH_DOWN;
	else if (m_vision_angle >= M_3_PI_4 && m_vision_angle <= M_5_PI_4) return WATCH_LEFT;
	else return WATCH_TOP;
}

void Entity::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (m_animation_ptr && m_current_shader_ptr) target.draw(*m_animation_ptr, m_current_shader_ptr);
	else if (m_animation_ptr) target.draw(*m_animation_ptr);

	// Drawing hitboxes
	//sf::RectangleShape rc;
	//rc.setFillColor(sf::Color::Transparent);
	//rc.setOutlineThickness(-1);
	//for (auto it = m_hitboxes_list.begin(); it != m_hitboxes_list.end(); it++)
	//{
	//	rc.setOutlineColor((it->getDamageMultiply() > 1.f) ?  sf::Color::Red : sf::Color::White);
	//	rc.setPosition(sf::Vector2f(this->getVisibleBounds().left, this->getVisibleBounds().top) + it->getOffset());
	//	rc.setSize(it->getSize());
	//	target.draw(rc);
	//}
}
