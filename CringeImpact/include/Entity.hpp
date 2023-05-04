#pragma once
#include "SFML/Graphics.hpp"
#include "MapObject.hpp"
#include "Solid.hpp"

class Entity : public MapObject
{
protected:

	enum Movement : uint8_t
	{
		MOVE_TOP,
		MOVE_RIGHT,
		MOVE_LEFT,
		MOVE_BOTTOM,
		STAY_RIGHT,
		STAY_LEFT
	};

	enum Death : uint8_t
	{
		DIE_RIGHT,
		DIE_LEFT
	};

	enum Fight : uint8_t
	{
		ATTACK_TOP,
		ATTACK_RIGHT,
		ATTACK_BOTTOM,
		ATTACK_LEFT
	};

	enum VisionDir : uint8_t
	{
		WATCH_RIGHT,
		WATCH_DOWN,
		WATCH_LEFT,
		WATCH_TOP
	};
	
public:
	Entity();
	void setMaxHP(uint32_t value) { m_max_hp = value; };
	void setHP(uint32_t value) { m_current_hp = value; };
	void setMaxMP(uint32_t value) { m_max_mp = value; };
	void setMP(uint32_t value) { m_current_mp = value; };
	void setMaxStamina(uint32_t value) { m_max_stamina = value; };
	void setStamina(uint32_t value) { m_current_stamina = value; };
	void move(sf::Vector2f delta);
	void attack();
	void die();
	void takeDamage(float damage);
	bool isDead();
	virtual void update(float tick) override;
protected:
	VisionDir getVisionSector();

	bool m_is_attack_sound_playing;
	sf::Sound* m_attack_sound_ptr;
	sf::Sound* m_death_sound_ptr;

	uint32_t m_max_hp;
	uint32_t m_current_hp;
	uint32_t m_max_mp;
	uint32_t m_current_mp;
	uint32_t m_max_stamina;
	uint32_t m_current_stamina;

	Animation m_move_anim;
	Animation m_die_anim;
	Animation m_attack_anim;
	Movement m_last_move_dir;

	float m_speed;
	bool m_is_attacking;
	bool m_is_dead;
	float m_vision_angle;
	bool m_is_damaged_recently;

	static bool m_is_damaged_shader_loaded;
	static sf::Shader m_damaged_shader;
	sf::Shader* m_current_shader_ptr;
	float m_damaged_timer;

	Entity* m_target;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};