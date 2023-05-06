#pragma once
#include "SFML/Graphics.hpp"
#include "MapObject.hpp"
#include "Hitbox.hpp"

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

	// Setters
	void setMaxHP(float value) { m_max_hp = value; };
	void setHP(float value) { m_current_hp = value; };
	void setMaxMP(float value) { m_max_mp = value; };
	void setMP(float value) { m_current_mp = value; };
	void setMaxStamina(float value) { m_max_stamina = value; };
	void setStamina(float value) { m_current_stamina = value; };

	// Getters
	float getMaxHP() { return m_max_hp; };
	float getHP() { return m_current_hp; };
	float getMaxMP() { return m_max_mp; };
	float getMP() { return m_current_mp; };
	float getMaxStamina() { return m_max_stamina; };
	float getStamina() { return m_current_stamina; };
	float getVisionAngle() { return m_vision_angle; };

	// Return Hitbox pointer if one of the hitboxes contains attack_point
	// Else return NULL 
	Hitbox* getAttackedHitbox(sf::Vector2f attack_point);

	void move(sf::Vector2f delta);
	void attack(sf::Vector2f attack_point, std::list<Entity*>& entitys_list);
	void die();
	void takeDamage(float damage);
	bool isDead();
	virtual void update(float tick) override;
protected:
	VisionDir getVisionSector();

	std::list<Hitbox> m_hitboxes_list;

	bool m_is_attack_sound_playing;
	sf::Sound* m_attack_sound_ptr;
	sf::Sound* m_death_sound_ptr;
	sf::Sound* m_walk_sound_ptr;
	sf::Sound* m_hurt_sound_ptr;

	float m_max_hp;
	float m_current_hp;
	float m_max_mp;
	float m_current_mp;
	float m_max_stamina;
	float m_current_stamina;

	Animation m_move_anim;
	Animation m_die_anim;
	Animation m_attack_anim;
	Movement m_last_move_dir;

	float m_speed;
	float m_attack_range;
	float m_attack_damage;

	bool m_is_attacking;
	bool m_is_dead;
	float m_vision_angle;
	bool m_is_damaged_recently;

	static bool m_is_damaged_shader_loaded;
	static sf::Shader m_damaged_shader;
	sf::Shader* m_current_shader_ptr;
	float m_damaged_timer;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};